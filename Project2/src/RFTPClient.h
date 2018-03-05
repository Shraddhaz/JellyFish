#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include "Packet.h"


class RFTPClient{
	private:
		int sock, sock_ack;                                                //Socket
		unsigned int length;                                               //Unsigned int for length
		struct sockaddr_in server, server_ack;             //Client and server struct
   		struct hostent *hp;                                                //Host net struct

	public:
		RFTPClient();                                                       //RFTP Client Constructor
		~RFTPClient();                                                      //RFTP Client Destructor
		int connectAndSend(char * hostname);	                            //Connect to Server
		int sendRequest();                                                  //Send request to client
		void receivePacket();                                               //Receive packet from server
		bool requestFile(char *filename);                                   //Request file from Server
		void send_packet(PacketKind pk, int seq_no);                        //Send packet
		void send_packet(PacketKind pk, int seq_no, int size, void *data);  //Send packet with data
};
