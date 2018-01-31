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

class RFTPClient{
	private:
		int sock, n;
		unsigned int length;
   		struct sockaddr_in server, from;
   		struct hostent *hp;
   		char buffer[256];

	public:
		RFTPClient();
		~RFTPClient();
		int connectAndSend();	
		int sendRequest();
		void receivePacket();
		bool requestFile(char *filename);
};
