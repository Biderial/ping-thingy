This program is an attempt to recreate the ping command.
It makes use of windows libraries to first,
check to see if the version is compatible,
create a socket,
define parameters from from the SOCKADDR data structures windows uses,
set an amount of packets to be sent,
loop through sending and receiving data out of a buffer of icmp data structure and into a buffer containing data from icmp replies,
and to close the socket and terminate the ws2_32 lib.
