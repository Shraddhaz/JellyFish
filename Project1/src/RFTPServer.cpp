#include "RFTPServer.h"

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

RFTPServer::RFTPServer(){
	sock=socket(AF_INET, SOCK_DGRAM, 0);
   	
	if (sock < 0) error("Opening socket");
   		length = sizeof(server);
   	bzero(&server,length);
   	
	server.sin_family=AF_INET;
   	server.sin_addr.s_addr=INADDR_ANY;
   	server.sin_port=htons(PORT_NUMBER);
}

void RFTPServer :: Bind(){
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   printf("Bind complete");
}

void RFTPServer::ListenAccept(){
    printf("In Listen and accept");
	char c = 'c';
	void * p = &c;
	Packet packet = Packet(CONNECTION_ACK, 1, p);
	void * vptr = packet.serialize();
    while (1) {
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       write(1,"Received a datagram: ",21);
       n = sendto(sock, vptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
	}
}
