#include "RFTPClient.h"

using namespace std;

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
	if (hp==0) return -1;	
	
	bcopy((char *)hp->h_addr,
		  (char *)&server.sin_addr,
          hp->h_length);
	server.sin_port = htons(PORT_NUMBER);
   	length=sizeof(struct sockaddr_in);
	bzero(buffer,256);
	return 1;
}

int RFTPClient::sendData(char * data)
{
	void *buf = malloc(PACKET_SIZE);
	bzero(buf, PACKET_SIZE);
	Packet p = Packet(CONNECTION_REQUEST, 0, buf);
	n=sendto(sock, buf, PACKET_SIZE, 0,(const struct sockaddr *)&server,length);
	if (n < 0) printf("Sendto");
	n = recvfrom(sock, buf,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
	Packet packet = new Packet(buf);
	packet.printPacket();
	if (n < 0) printf("recvfrom");
}
