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
	
	Packet p = Packet(CONNECTION_REQUEST, 0, 0, buf);	
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
	int len = strlen(filename);
	memset(vfilename, 0, DATA_SIZE);
	memcpy(vfilename, filename, len);
	Packet pack = Packet(FILE_REQUEST, 0, len, vfilename);
	void * buffer = pack.serialize();
	void *ptr, *ptr2;
	int x;
	n=sendto(sock, buffer, PACKET_SIZE, 0,(const struct sockaddr *)&server,length);
	//cout<<"Checkpoint 1\n";
	if (n < 0)
        return false;
	//cout<<"Checkpoint 2\n";

	ptr = malloc(PACKET_SIZE);
    n = recvfrom(sock, ptr,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
	//cout<<"Checkpoint 3\n";
	if (n < 0)
		return false;
	//cout<<"Checkpoint 4\n";
    Packet packet = Packet(ptr);
    packet.printPacket();
	
	cout<<"Creating file.\n";
	int fdWrite = open("./testWrite.txt", O_CREAT | O_TRUNC| O_WRONLY, 0644);
	while (1) {
		 n = recvfrom(sock, ptr2,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
		if (n<= 0) break;
		Packet pack = Packet(ptr2);
		x = write(fdWrite, pack.data, pack.sizeOfData);
		void *ptr3 = malloc(DATA_SIZE);
		memset(ptr3, 0, DATA_SIZE);
		Packet pack2 = Packet(DATA_ACK, 0, 0, ptr3);
		sendto(sock, pack2.serialize(), PACKET_SIZE, 0,(const struct sockaddr *)&server,length);
	}
	return true;
}
