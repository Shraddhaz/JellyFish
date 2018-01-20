#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Constants.h"

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
		int connectServer();	
		int sendData(char* data);
};
