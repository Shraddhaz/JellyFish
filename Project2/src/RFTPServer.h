/**
RFTPServer.h has the class RFTPServer which holds all the data members
used by the Server for its implementation and all the server methods
like binding, listening for requests, accepting requests, sending and
receiving packets
*/

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include "Packet.h"
#include <iostream>
#include <fcntl.h>

class RFTPServer {
	private:
		int sock, sock_ack, length, length_ack;
   		socklen_t fromlen;                                         //Client length
   		struct sockaddr_in server;                                              //Server's socket struct
		struct sockaddr_in server_ack;
   		struct sockaddr_in from;                                                //Client's socket struct
		struct sockaddr_in from_ack;
		bool isConnected;                                                       //Connection established or not
		struct timeval read_timeout;                                            //Timeout for reading data
		struct timeval reset_timeout;                                           //Timeout after reset

	public:
		RFTPServer();                                                           //RFTPServer Constructor
		void Bind();                                                            //Binding Server socket to Portno and IP
		void ListenAccept();                                                    //Listen to request and accept
		void receivePacket();                                                   //Receive packet and deserialize it
		bool fileReq(void * ptr, int size);                                     //Handle retransmission and timeout
		void send_packet(PacketKind pk, int sn);                                //Send packet
		void send_packet(PacketKind pk, int sn, int size, void *data);          //Send packet with data
};
