#include "RFTPClient.h"
using namespace std;

/**
RFTP Client constructor that creates a socket
*/
RFTPClient::RFTPClient()
{
	serverS.sin_family = AF_INET;
	serverR.sin_family = AF_INET;
	sockR = socket(AF_INET, SOCK_DGRAM, 0);
	sockS = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockR < 0||sockS <0)
		cout<<"Error creating socket."<<endl;
}
/**
RFTPClient destructor that closes the socket
*/
RFTPClient::~RFTPClient()
{
	close(sockS);
	close(sockR);
}

/**
connectAndSend() sends a connection request to the server and
establishes a connection with the server
@param hostname is the hostname of the server
@return 0 if the connection is success else 1
*/
int RFTPClient::connectAndSend(char * hostname)
{
	hp = gethostbyname(hostname);
	if (hp==0) return -1;	//Host not found.	
	
	memcpy((char *)&serverR.sin_addr, (char *)hp->h_addr, hp->h_length);
	serverR.sin_port = htons(PORT_NUMBER_ACK);
   	length=sizeof(struct sockaddr_in);

	//Send request for connection
    send_packet(sockS, CONNECTION_REQUEST, 0);
	void *received_packet = malloc(PACKET_SIZE);
	memset(received_packet, 0, PACKET_SIZE);
	int n = recvfrom(sockS, received_packet,  PACKET_SIZE, 0, (struct sockaddr *)&serverS, &length);
	Packet packet = Packet(received_packet);

	//Checks if acknowledgement is received
	if(packet.kind != CONNECTION_ACK) {
		return 0;
	}
	//Deleting the acknowledgement
	delete(received_packet);
	
	if (n < 0)
		return 0;
	return 1;
}

/**
requestFile() is the function that sends a FILE_REQUEST packet
requesting for a file from the server
@param filename is the file name to be send fro the client
@return boolean stating whether request sent successfully or not
*/
bool RFTPClient::requestFile(char *filename)
{
	//Copying filename on the data field of packet.
	void *vfilename = malloc(DATA_SIZE);
	int len = strlen(filename)+1;
	memset(vfilename, 0, DATA_SIZE);
	memcpy(vfilename, filename, len);

	bool return_val = false;
	//Create absolute path of the filename
	char abs_filename[(6+1+len)];
	strcpy(abs_filename, client_fs);
	strcat(abs_filename, "/");
	strcat(abs_filename, filename);

	void *received_packet;
	int x;
	//Sending file request packet to serverS
	send_packet(FILE_REQUEST, 2, len, vfilename);

	received_packet = malloc(PACKET_SIZE);
    recvfrom(sockS, received_packet,  PACKET_SIZE, 0, (struct sockaddr *)&serverR, &length);
    Packet packet = Packet(received_packet);

	//Wating for acknowledgement clientS Server
	if (packet.kind != FILE_REQUEST_ACK) {
		return false;
	}
	
	memset(received_packet, 0, PACKET_SIZE);

	//Client ready for data transfer
    send_packet(sockR, START_DATA_TRANSFER, 20);

    //Receiving file
	cout<<"Creating file.\n";
	bool flag = true;
	int previousPacketNo = 3;
	int fdWrite = open(abs_filename, O_CREAT | O_TRUNC| O_WRONLY, 0644);
	while (1) {
		int n = recvfrom(sockR, received_packet,  PACKET_SIZE, 0, (struct sockaddr *)&serverS, &length);
		if (n<= 0) {
			return_val = false;
			break;
		}
		Packet pack = Packet(received_packet);
//		if(pack.sequence_number%1000 == 0) flag = !flag;
		cout<<"Received packet number: "<<pack.sequence_number<<endl;
		if(pack.kind == CLOSE_CONNECTION) {
			return_val = true;
			break;
		}
		else if (pack.kind != DATA) {
			return_val = false; 
			break;
		}
		if((previousPacketNo+1) == pack.sequence_number) write(fdWrite, pack.data, pack.sizeOfData);
		//if (flag)
        //    send_packet(sockS, DATA_ACK, 0);
		previousPacketNo = 	pack.sequence_number;
	}

	//Free memory
	delete(received_packet);
	delete(vfilename);
	return return_val;
}

/**
send_packet() is used to send packet to the server without data.
Type of packet sent here is mostly an acknowledgement packet
@PacketKind is the type of packet being send
@seq_no is the sequence number of the packet
*/
void RFTPClient::send_packet(int socket, PacketKind pk, int seq_no) {
	void *data = malloc(DATA_SIZE);
	memset(data, 0, DATA_SIZE);
    Packet packet = Packet(pk, seq_no, 0, data);
    void *ptr = packet.serialize();
    sendto(socket, ptr, PACKET_SIZE, 0,(const struct sockaddr *)&serverR, length);
	delete(data);
    delete(ptr);
}

/**
send_packet() is used to send packet to the server with data.
Type of packet sent here is mostly a data packet
@param PacketKind is the type of packet being send
@param seq_no is the sequence number of the packet
@param data is the data being sent by the client
*/
void RFTPClient::send_packet(PacketKind pk, int seq_no, int size, void *data) {
    Packet packet = Packet(pk, seq_no, size, data);
    void *ptr = packet.serialize();
    sendto(sockS, ptr, PACKET_SIZE, 0,(const struct sockaddr *)&serverR,length);
    delete(ptr);
}
