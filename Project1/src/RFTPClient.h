#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Packet.h"
#include <iostream>
#include <fcntl.h>

class RFTPClient{
	private:
		int sock;
		unsigned int length;
		struct sockaddr_in server, from; 
   		struct hostent *hp; 

	public:
		RFTPClient();
		~RFTPClient();
		int connectAndSend(char * hostname);	
		int sendRequest();
		void receivePacket();
		bool requestFile(char *filename);
		void send_packet(PacketKind pk, int seq_no);
		void send_packet(PacketKind pk, int seq_no, int size, void *data);
};
