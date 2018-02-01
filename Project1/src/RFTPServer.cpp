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
   	memset(&server,0,length);
	
	server.sin_family=AF_INET;
   	server.sin_addr.s_addr=INADDR_ANY;
   	server.sin_port=htons(PORT_NUMBER);
	isConnected = false;
	buf = malloc(PACKET_SIZE);
}

void RFTPServer :: Bind(){
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   printf("Bind complete\n");
}

void RFTPServer::ListenAccept(){
	cout<<"Connection Request Received\n";
	void *ptr = malloc(DATA_SIZE);
	memset(ptr,0,DATA_SIZE);
	Packet packet = Packet(CONNECTION_ACK, 1, ptr);
	void * vptr = packet.serialize();
    n = sendto(sock, vptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
	cout<<"Connection Acknowledgement Sent\n";
	this->isConnected = true;
}

void RFTPServer::fileReq(void *vfilename)
{
	int len =0;

	memcpy(&len, vfilename, sizeof(int));
	char *filename = (char *) malloc(len);
	
	memcpy(filename, (vfilename + sizeof(int)), len);		
	cout<<"Filename requested: "<<filename<<endl;

	void *ptr123 = malloc (DATA_SIZE);
	memset(ptr123, 0, DATA_SIZE);
	Packet pack1 = Packet(FILE_REQUEST_ACK, 0, ptr123);
	void *tempptr = pack1.serialize();
	sendto(sock, tempptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
	
	cout<<"File req ack sent\n";
	void *buffer=malloc(3);
	int n = 0;
	int fdRead = open("./ServerFileSystem/test.txt", 'r');
	while(1) {
		if((n = read(fdRead, buffer, 3)) <= 0) break;
		Packet pack = Packet(DATA, 0, buffer);
		void *ptr = pack.serialize();
		n = sendto(sock, ptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
		n = recvfrom(sock,buf,PACKET_SIZE,0,(struct sockaddr *)&from,&fromlen);
	}
}

void RFTPServer::receivePacket(){
	//n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
    while(1) {
		if (n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen) < 0) 
		cout<<"Nothing read from socket"<<endl;
        else
        {
         	Packet packet = Packet(buf);
			switch(packet.kind){
			case CONNECTION_REQUEST:
				ListenAccept();
				break;
			case FILE_REQUEST:
				fileReq(packet.data);
				break;	
			default:
				cout<<"Matter Zhalay Bhau!"<<endl; 
		}

        }
	}
}
