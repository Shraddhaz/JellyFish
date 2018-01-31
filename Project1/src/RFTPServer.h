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
class RFTPServer {
	private:
		int sock, length, n;
   		socklen_t fromlen;
   		struct sockaddr_in server;
   		struct sockaddr_in from;
		void *buf;
		bool isConnected;
	public:
		RFTPServer();
		//~RFTPServer();
		void Bind();
		void ListenAccept();
		void receivePacket();
		void fileReq(void *vfileName);		
};
