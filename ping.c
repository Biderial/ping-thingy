#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")

#define OUTGOING_PACKET_SIZE 9
#define ICMP_PORT 0
#define INCOMING_PACKET_SIZE 1024

// Data structures
typedef struct ICMP_PACKET{
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t icmp_checksum;
    uint16_t icmp_id;
    uint16_t icmp_seq_num;
} ICMP_PACKET;

// Prototypes
uint16_t checksum(uint16_t *buf, int len);


int main(void){

    WORD version = MAKEWORD(2,2);
    WSADATA wsaData;

    int err;
    err = WSAStartup(version, &wsaData);
    if (err != 0){
        WSACleanup();
    }
    else {
        printf("Device compatible\n");
    }

    // Socket creation
    SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == INVALID_SOCKET){
        printf("socket failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Designate dest info
    SOCKADDR_IN dstaddr; 
    dstaddr.sin_family = AF_INET;
    dstaddr.sin_addr.s_addr = inet_addr("8.8.8.8"); // Change from static to dynamic later on
    dstaddr.sin_port = 0;

    printf("How many packets?: \n");
    int amount = 0;
    scanf("%d", &amount);

    for (int i = 0; i < amount; i++){
        char packet[OUTGOING_PACKET_SIZE];
        ICMP_PACKET *icmpHeader = (ICMP_PACKET *)packet; // Typecast individual bytes into a datastructure
        icmpHeader->icmp_type = 8; // Static (change to dynamic)
        icmpHeader->icmp_code = 0; // Static (change to dynamic)
        icmpHeader->icmp_checksum = 0; 
        icmpHeader->icmp_id = (uint16_t)GetCurrentProcessId();
        icmpHeader->icmp_seq_num = i+1;
        icmpHeader->icmp_checksum = checksum((uint16_t *)packet, OUTGOING_PACKET_SIZE); // Checksum must be done after packet completion

        // Send each packet
        int sentbytes = sendto(sock, (char *)icmpHeader, OUTGOING_PACKET_SIZE, 0, (SOCKADDR*) &dstaddr, sizeof(dstaddr));  // Change flags 
        if (sentbytes == SOCKET_ERROR){
            printf("sendto failed with error: %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        char incoming_data[INCOMING_PACKET_SIZE];
        SOCKADDR incoming_sender;
        int incoming_sender_len = sizeof(incoming_sender);

        int returned_bytes = recvfrom(sock, incoming_data, INCOMING_PACKET_SIZE, 0, &incoming_sender, &incoming_sender_len); // Change flags 

        // TODO 
        // Interpret data received

    }

    closesocket(sock);

    WSACleanup();

    return 0;

}


uint16_t checksum(uint16_t *buf, int len) {
    uint32_t sum = 0;
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(uint8_t *)buf;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}