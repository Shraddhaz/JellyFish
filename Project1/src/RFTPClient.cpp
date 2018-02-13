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

int RFTPClient::connectAndSend(char * hostname)
{
	hp = gethostbyname(hostname);
	if (hp==0) return -1;	
	
	memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);
	server.sin_port = htons(PORT_NUMBER);
   	length=sizeof(struct sockaddr_in);
	
	send_packet(CONNECTION_REQUEST, 0);
	void *received_packet = malloc(PACKET_SIZE);
	memset(received_packet, 0, PACKET_SIZE);
	int n = recvfrom(sock, received_packet,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
	Packet packet = new Packet(received_packet);
	//packet.printPacket();

	delete(received_packet);
	
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
	//Copying filename on the data field of packet.
	void *vfilename = malloc(DATA_SIZE);
	int len = strlen(filename)+1;
	memset(vfilename, 0, DATA_SIZE);
	memcpy(vfilename, filename, len);
	//Packet pack = Packet(FILE_REQUEST, 2, len, vfilename);
	//void * serialized_packet = pack.serialize();	//Freed

	//Create absolute path of the filename
	char abs_filename[(6+1+len)];
	strcpy(abs_filename, client_fs);
	strcat(abs_filename, "/");
	strcat(abs_filename, filename);

	void *received_packet;
	int x;
	//int n=sendto(sock, serialized_packet, PACKET_SIZE, 0,(const struct sockaddr *)&server,length);
	send_packet(FILE_REQUEST, 2, len, vfilename);
	//delete(serialized_packet);
	//cout<<"Checkpoint 1\n";
	//cout<<"Checkpoint 2\n";

	received_packet = malloc(PACKET_SIZE);
    recvfrom(sock, received_packet,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
	//cout<<"Checkpoint 3\n";
	//cout<<"Checkpoint 4\n";
    Packet packet = Packet(received_packet);
    //packet.printPacket();	
	memset(received_packet, 0, PACKET_SIZE);
	
	send_packet(START_DATA_TRANSFER, 20);

	cout<<"Creating file.\n";
	bool flag = true;
	int previousPacketNo = 0;
	int fdWrite = open(abs_filename, O_CREAT | O_TRUNC| O_WRONLY, 0644);
	while (1) {
		int n = recvfrom(sock, received_packet,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
		if (n<= 0) break;
		Packet pack = Packet(received_packet);

		if(pack.sequence_number%1000 == 0) flag = !flag;
		//cout<<"Received packet in infinite loop.\n";	
		//pack.printPacket();
		cout<<"Received packet number: "<<pack.sequence_number<<endl;
		if(pack.kind == CLOSE_CONNECTION)
			break;
		if(previousPacketNo != pack.sequence_number) write(fdWrite, pack.data, pack.sizeOfData);
		if (flag) send_packet(DATA_ACK, 0);
		previousPacketNo = 	pack.sequence_number;
	}
	
	/*
	void *ptr = malloc(PACKET_SIZE); 
	n = recvfrom(sock, ptr,  PACKET_SIZE, 0, (struct sockaddr *)&from, &length);
	Packet p = Packet(ptr);
	if(p.kind == CLOSE_CONNECTION)
		exit(0);	
	*/
	delete(received_packet);
	delete(vfilename);
	return true;
}

bool RFTPClient::send_packet(PacketKind pk, int seq_no) {
	void *data = malloc(DATA_SIZE);
	memset(data, 0, DATA_SIZE);
    Packet packet = Packet(pk, seq_no, 0, data);
    void *ptr = packet.serialize();
    sendto(sock, ptr, PACKET_SIZE, 0,(const struct sockaddr *)&server,length);
	delete(data);
    delete(ptr);
}

bool RFTPClient::send_packet(PacketKind pk, int seq_no, int size, void *data) {
    Packet packet = Packet(pk, seq_no, size, data);
    void *ptr = packet.serialize();
    sendto(sock, ptr, PACKET_SIZE, 0,(const struct sockaddr *)&server,length);
    delete(ptr);
}
