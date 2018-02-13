#include "RFTPServer.h"

using namespace std;

RFTPServer::RFTPServer(){
	sock=socket(AF_INET, SOCK_DGRAM, 0);
   	
	if (sock < 0) cout<<"Opening socket"<<endl;
   		length = sizeof(server);
   	memset(&server,0,length);
	
	server.sin_family=AF_INET;
   	server.sin_addr.s_addr=INADDR_ANY;
   	server.sin_port=htons(PORT_NUMBER);


	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 100000;

	isConnected = false;
}

void RFTPServer :: Bind(){
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       cout<<"binding"<<endl;
   fromlen = sizeof(struct sockaddr_in);
   cout<<"Bind complete\n";
}

void RFTPServer::ListenAccept(){
	cout<<"Connection Request Received\n";
	cout<<"Pointer ptr created.";
	send_packet(CONNECTION_ACK, 1);
	cout<<"Connection Acknowledgement Sent\n";
	this->isConnected = true;
}

bool RFTPServer::fileReq(void *vfilename, int size_of_data)
{
	cout<<"In file req.\nSize of data is: "<<size_of_data;
	
	if (!this->isConnected)
		return false;
	
	char absfilename[6+1+size_of_data];
	char filename[size_of_data];
	memcpy(filename, vfilename, size_of_data);		
	cout<<"Filename requested: "<<filename<<endl;
	strcpy(absfilename, server_fs);
	strcat(absfilename, "/");
	strcat(absfilename, filename);
	cout<<absfilename<<endl;

	if( access(absfilename, F_OK ) == -1 ) {
		return false;
	}	

	int fdRead = open(absfilename, 'r');
	cout<<"fdRead is: "<<fdRead<<endl;
	
	//Create a data packet
	send_packet(FILE_REQUEST_ACK, 3);	
	cout<<"File req ack sent\n";
	
	void *data = malloc(DATA_SIZE);
	memset(data, 0, DATA_SIZE);

	void *tempptr = malloc(PACKET_SIZE);
	recvfrom(sock,tempptr,PACKET_SIZE,0,(struct sockaddr *)&from,&fromlen);
	Packet p = Packet(tempptr);
	if (p.kind != START_DATA_TRANSFER)
		return false;	
	
	//TODO: Change the datasn. !!!!!!!!!!!DO NOT START FROM HARDCODED SEQUENCE NUMBER!!!!!!!!!!!!!!!!!!	
	int bytesRead = 0;
	int datasn = 4;

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

	while(1) {
		if((bytesRead = read(fdRead, data, DATA_SIZE)) <= 0) break;
		void *ptr = malloc(PACKET_SIZE);	
		Packet *temp;
		bool cond = true;
		do {
			send_packet(DATA, datasn, bytesRead, data);
			memset(ptr, 0, PACKET_SIZE);
	
			int n = recvfrom(sock,ptr,PACKET_SIZE,0,(struct sockaddr *)&from,&fromlen);
			if (n < 0) continue;
				cout<<"Received Packet.\n";

			temp = new Packet(ptr);
			cond = temp->kind != DATA_ACK;
			delete(temp);
		} while(cond);
		datasn++;
		delete(ptr);
	}

	cout<<"Sending close connection signal.\n";

	send_packet(CLOSE_CONNECTION, 0);
	delete(data);
	return true;
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
			packet.printPacket();
			switch(packet.kind){
			case CONNECTION_REQUEST:
				ListenAccept();
				break;
			case FILE_REQUEST:
				if (fileReq(packet.data, packet.sizeOfData))
					cout<<"File Request function done.\n";
				else
					send_packet(FILE_REQ_ERROR, -1);
				break;	
			default:
				cout<<"Could not recognize the packet kind and thus ignoring the packet."<<endl; 
		}

        }
	}
}

void RFTPServer::send_packet(PacketKind pk, int seq_no) {
	void *data = malloc(DATA_SIZE);
	memset(data, 0, DATA_SIZE);
	Packet packet = Packet(pk, seq_no, 0, data);
	void *ptr = packet.serialize();
	sendto(sock, ptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
	delete(data);	
	delete(ptr);	
}

void RFTPServer::send_packet(PacketKind pk, int seq_no, int size, void *data) {
	Packet packet = Packet(pk, seq_no, size, data);
	void *ptr = packet.serialize();
	sendto(sock, ptr, PACKET_SIZE,0,(struct sockaddr *)&from,fromlen);
	delete(ptr);	
}
