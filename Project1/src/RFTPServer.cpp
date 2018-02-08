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
}

void RFTPServer :: Bind(){
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   printf("Bind complete\n");
}

void RFTPServer::ListenAccept(){
	cout<<"Connection Request Received\n";
	void *ptr  = malloc(DATA_SIZE);
	cout<<"Pointer ptr created.";
	memset(ptr,0,DATA_SIZE);
	Packet packet = Packet(CONNECTION_ACK, 1, 0, ptr);
	void * vptr = packet.serialize();
    int n = sendto(sock, vptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
	delete(vptr);
	cout<<"Connection Acknowledgement Sent\n";
	this->isConnected = true;
}

void RFTPServer::fileReq(void *vfilename, int size_of_data)
{
	cout<<"In file req.\nSize of data is: "<<size_of_data;
	
	if (!this->isConnected)
		return;
	
	//Read filename
	char *filename = (char *) malloc(size_of_data);  //Freed
	memcpy(filename, vfilename, size_of_data);		
	cout<<"Filename requested: "<<filename<<endl;

	//Create a data packet
	void *data = malloc (DATA_SIZE);  //Freed
	memset(data, 0, DATA_SIZE);
	Packet pack1 = Packet(FILE_REQUEST_ACK, 0, 0, data);
	void *serialized_packet = pack1.serialize();	//Freed
	sendto(sock, serialized_packet, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
	
	cout<<"File req ack sent\n";
	
	//void *buffer = malloc(DATA_SIZE);
	memset(data, 0, DATA_SIZE);
	
	int bytesRead = 0;

	// TODO: Change this to read file specified by client.
	int fdRead = open("./ServerFileSystem/test.txt", 'r');
	while(1) {
		if((bytesRead = read(fdRead, data, DATA_SIZE)) <= 0) break;
		Packet pack = Packet(DATA, 0, bytesRead, data);
		void *ptr = pack.serialize();	//Freed 
		int n = sendto(sock, ptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
		delete(ptr);
		//memset(ptr, 0, PACKET_SIZE);
		n = recvfrom(sock,ptr,PACKET_SIZE,0,(struct sockaddr *)&from,&fromlen);
	}
	delete(filename);
	delete(data);
	delete(serialized_packet);
}

void RFTPServer::receivePacket(){
	void *buf = malloc(PACKET_SIZE);  //To read a packet from socket.
	int n; //Number of bytes read.
    while(1) {
		if (n = recvfrom(sock,buf, PACKET_SIZE,0,(struct sockaddr *)&from,&fromlen) < 0) 
		cout<<"Nothing read from socket"<<endl;
        else
        {
         	Packet packet = Packet(buf);
			switch(packet.kind){
			case CONNECTION_REQUEST:
				ListenAccept();
				break;
			case FILE_REQUEST:
				fileReq(packet.data, packet.sizeOfData);
				break;	
			default:
				cout<<"Matter Zhalay Bhau!"<<endl; 
		}

        }
	}
}
