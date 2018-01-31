#include "RFTPClient.h"

using namespace std;

RFTPClient::RFTPClient()
{
	server.sin_family = AF_INET;
	sock= socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) 
		cout<<"socket"<<endl;
}

RFTPClient::~RFTPClient()
{
	close(sock);
}

int RFTPClient::connectAndSend()
{
	hp = gethostbyname(HOSTNAME);
	if (hp==0) return -1;	
	
	memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);
	server.sin_port = htons(PORT_NUMBER);
   	length=sizeof(struct sockaddr_in);
	memset(buffer,0,256);

	void *buf = malloc(PACKET_SIZE);
	memset(buf, 0,PACKET_SIZE);
	
	Packet p = Packet(CONNECTION_REQUEST, 0, buf);	
	n=sendto(sock, buf, PACKET_SIZE, 0,(const struct sockaddr *)&server,length);
	if (n < 0)
		return 0;
	
	n = recvfrom(sock, buf,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
	Packet packet = new Packet(buf);
	packet.printPacket();
	if (n < 0)
		return 0;
	return 1;
}

void RFTPClient::receivePacket(){
 	while(1){
 	
 		
 	}
 
 }

bool RFTPClient::requestFile(char *filename)
{
	void *vfilename = malloc(DATA_SIZE);
	int length = strlen(filename);
	memset(vfilename, 0, DATA_SIZE);
	memcpy(vfilename, &length, sizeof(int));
	memcpy((vfilename+sizeof(int)), filename, length);
	Packet pack = Packet(FILE_REQUEST, 0, vfilename);
	return true;
}
