#include "RFTPClient.h"

RFTPClient::RFTPClient()
{
	server.sin_family = AF_INET;
	sock= socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) printf("socket");
}

RFTPClient::~RFTPClient()
{
	close(sock);
}

int RFTPClient::connectServer()
{
	hp = gethostbyname(HOSTNAME);
	if (hp==0) printf("Unknown host");	
	
	bcopy((char *)hp->h_addr,
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(PORT_NUMBER);
   length=sizeof(struct sockaddr_in);
}

int RFTPClient::sendData(char * data)
{
	n=sendto(sock, data, strlen(data), 0,(const struct sockaddr *)&server,length);
	if (n < 0) printf("Sendto");
	n = recvfrom(sock, data, 256, 0, (struct sockaddr *)&from, &length);
   	if (n < 0) printf("recvfrom");
   	write(1,"Got an ack: ",12);
   	write(1,buffer,n);
}
