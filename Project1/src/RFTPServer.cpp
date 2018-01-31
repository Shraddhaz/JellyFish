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
    //printf("In Listen and accept\n");
	cout<<"Connection Request Received\n";
	void *ptr = malloc(DATA_SIZE);
	memset(ptr,0,DATA_SIZE);
	Packet packet = Packet(CONNECTION_ACK, 1, ptr);
	void * vptr = packet.serialize();
    //while (1) {
      // n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       //if (n < 0) cout<<"Nothing read from socket"<<endl;
		//else 
	//	{
			//Packet packet = Packet(buf);
			//if (packet.kind == CONNECTION_REQUEST) break; 
		//}
	//}
	//cout<<"Connection request accepted!\n";
      	n = sendto(sock, vptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
	cout<<"Connection Acknowledgement Sent\n";
	this->isConnected = true;
}

void RFTPServer::receivePacket(){
	//n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
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
	
		default:
			cout<<"Matter Zhalay Bhau!"<<endl; 
		}

        }
}


