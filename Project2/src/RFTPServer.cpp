#include "RFTPServer.h"
using namespace std;

/**
RFTPServer constructor used to create a socket and initialize
 all the values used by the server
*/
RFTPServer::RFTPServer(){
	sockS = socket(AF_INET, SOCK_DGRAM, 0);
   	sockR  = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockS < 0 || sockR < 0)
		cout<<"Error Opening Socket"<<endl;
   	length = sizeof(serverS);
   	memset(&serverS, 0, length);
	
	lengthAck = sizeof(serverR);
    memset(&serverR, 0, lengthAck);
	
	serverS.sin_family=AF_INET;
   	serverS.sin_addr.s_addr=INADDR_ANY;
   	serverS.sin_port=htons(PORT_NUMBER_DATA);

	serverR.sin_family=AF_INET;
    serverR.sin_addr.s_addr=INADDR_ANY;
    serverR.sin_port=htons(PORT_NUMBER_ACK);

	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 100000;

	reset_timeout.tv_sec = 0;
	reset_timeout.tv_usec = 0;

	isConnected = false;
}

/**
Bind() is used to bind the socket with the socket struct i.e. Port number and IP address
*/
void RFTPServer :: Bind(){
   if ( (bind(sockS,(struct sockaddr *)&serverS,length)<0) || (bind(sockR,(struct sockaddr *)&serverR,lengthAck)<0))
       cout<<"Binding Error"<<endl;
   fromlen = sizeof(struct sockaddr_in);
   //fromlen_ack = sizeof(struct sockaddr_in);   
   cout<<"Bind complete\n";
}

/**
ListenAccept() is used to listen to client and accept connection request
*/
void RFTPServer::ListenAccept(){
	cout<<"Connection Request Received\n";
    send_packet(CONNECTION_ACK, 1);
	cout<<"Connection Acknowledgement Sent\n";
	this->isConnected = true;
}

/**
fileReq() is used for sending the requested file to the client and
handling error cases like retransmission or packet loss during the
file transfer.
@param vfilename is the file we transfer
@param size_of_data is the size of data theat we transfer
*/
bool RFTPServer::fileReq(uint8_t *vfilename, int size_of_data)
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

	this->fdRead = open(absfilename, 'r');
	cout<<"fdRead is: "<<this->fdRead<<endl;
	
	//Create a data packet
    send_packet(FILE_REQUEST_ACK, 3);
	//cout<<"File request ack sent"<<endl;
	uint8_t data[DATA_SIZE];
	memset(data, 0, DATA_SIZE);

	uint8_t tempptr[PACKET_SIZE];
	recvfrom(sockR, tempptr, PACKET_SIZE, 0, (struct sockaddr *)&clientR, &fromlen);
	Packet p = Packet(tempptr);
	if (p.kind != START_DATA_TRANSFER)
		return false;	
	
	//Call 2 threads.	
	pthread_t recvThread;
	pthread_create(&recvThread, NULL, receiver, this);
	int bytesRead = 0;
	int datasn = 4;
	PWrap packetWrap;	
	int total_transmissions = 0;

	while(1) {
		memset(data, 0, DATA_SIZE);
		if((bytesRead = read(this->fdRead, data, DATA_SIZE)) <= 0) 
			break;
		bool cond = true;
		Packet packet = Packet(DATA, datasn, bytesRead, data);
		packetWrap.pack = &packet;
		packetWrap.s = RESEND;
		packetWrap.time = chrono::system_clock::now();	
		//Critical section begins
		pthread_mutex_lock(&(this->lock));
		if(packetMap.size() >= MAX_WINDOW_SIZE)
			pthread_cond_wait(&(this->isEmpty), &(this->lock));
		packetMap.insert({packetWrap.pack->sequence_number, packetWrap});
		cout<<"Inserting"<<packetWrap.pack->sequence_number<<endl;	
		//pthread_cond_signal(&(this->isEmpty));
		pthread_mutex_unlock(&(this->lock));
		this->send_packet(packet);
		total_transmissions++;
		datasn++;
		
	}
	//Sending Close Connection packet
	uint8_t ptr[PACKET_SIZE];
	memset(data, 0, DATA_SIZE);
    Packet packet = Packet(CLOSE_CONNECTION, datasn, 0, data);
    packet.serialize(ptr);
	//cout<<"Done sending all data packets. Sending Close connection now";
	packetWrap.pack = &packet;
    packetWrap.s = RESEND;
    packetWrap.time = chrono::system_clock::now();
    //Critical section begins

	pthread_mutex_lock(&(this->lock));
    if(packetMap.size() >= MAX_WINDOW_SIZE)
    	pthread_cond_wait(&(this->isEmpty), &(this->lock));
    packetMap.insert({packetWrap.pack->sequence_number, packetWrap});
    cout<<"Inserting close connection: "<<packetWrap.pack->sequence_number<<endl;
    //pthread_cond_signal(&(this->isEmpty));
    pthread_mutex_unlock(&(this->lock));
	
	sendto(this->sockS, ptr, PACKET_SIZE,0,(struct sockaddr *)&clientR,fromlen);
    //send_packet(CLOSE_CONNECTION, datasn);

	pthread_join(recvThread, NULL);
	cout<<"Number of re-transmissions: "<<(total_transmissions-(datasn-4))<<endl;
	cout<<"Sending close connection signal.\n";

	return true;
}

//Receiving ack
void* receiver(void* rcvargs) {
	int bytesRead = 0;
	int datasn = 4;
	RFTPServer *rtfpserver = (RFTPServer *) rcvargs; 
	//setsockopt(rtfpserver->sockR, SOL_SOCKET, SO_RCVTIMEO, &(rtfpserver->read_timeout), sizeof rtfpserver->read_timeout);
	int total_transmissions = 0;
	uint8_t data;
	std::unordered_map<int, PWrap>::const_iterator iter;
	bool isEnd = false;
	int size;
	
    while(1) {
		uint8_t ptr[PACKET_SIZE];
		Packet *temp;
		bool cond = true;
		do {
			total_transmissions++;
			memset(ptr, 0, PACKET_SIZE);
			
			int n = recvfrom(rtfpserver->sockR, ptr,PACKET_SIZE,0,(struct sockaddr *)&(rtfpserver->clientS), &(rtfpserver->fromlen));
			if (n < 0){
				continue;
				cout<<"Error in received Packet.\n";
			}
			temp = new Packet(ptr);
			cout<<"Received sequence no:"<<temp->sequence_number<<endl;
			pthread_mutex_lock(&(rtfpserver->lock));
            rtfpserver->packetMap.erase(temp->sequence_number);
            size = rtfpserver->packetMap.size();
			if(size == 1){
				auto it = rtfpserver->packetMap.begin();
				if(it->second.pack->kind==CLOSE_CONNECTION)
					isEnd = true;
			}
			pthread_cond_signal(&(rtfpserver->isEmpty));
            pthread_mutex_unlock(&(rtfpserver->lock));

			if(isEnd)
				break;

			cond = temp->kind != DATA_ACK;
			temp->printPacket();
		} while(cond);
		datasn++;
	}

}

/**
receivePacket() is used to receive packets like CONNECTION_REQUEST,
 FILE_REQUEST and call the appropriate functions to handle them
*/
void RFTPServer::receivePacket(){
	
	uint8_t buf[PACKET_SIZE];
	int n; //Number of bytes read.
    while(1) {
		bool isComplete = false;
		if (n = recvfrom(sockR, buf, PACKET_SIZE,0,(struct sockaddr *)&clientS,&fromlen) < 0)
		cout<<"Nothing read clientS socket"<<endl;
        else
        {
         	Packet packet = Packet(buf);
			packet.printPacket();
			switch(packet.kind){
			case CONNECTION_REQUEST:
				ListenAccept();
				break;
			case FILE_REQUEST:
				if (fileReq(packet.data, packet.sizeOfData)){
					cout<<"File Request function done.\n";
					isComplete = true;
				}
				else
                    send_packet(FILE_REQ_ERROR, -1);
				break;	
			default:
				cout<<"Could not recognize the packet kind and thus ignoring the packet."<<endl; 
			}
        }
		if(isComplete)
			break;
	}
}

/**
send_packet() is used to send packet to the server without data.
Type of packet sent here is mostly an acknowledgement packet
@PacketKind is the type of packet being send
@seq_no is the sequence number of the packet
*/
void RFTPServer::send_packet(PacketKind pk, int seq_no) {
	uint8_t data[DATA_SIZE];
	uint8_t ptr[PACKET_SIZE];

	memset(data, 0, DATA_SIZE);
	Packet packet = Packet(pk, seq_no, 0, data);
    packet.serialize(ptr);
	if(pk == CLOSE_CONNECTION)
		sendto(this->sockS, ptr, PACKET_SIZE,0,(struct sockaddr *)&clientR,fromlen);
	sendto(this->sockR, ptr, PACKET_SIZE,0,(struct sockaddr *)&clientS,fromlen);

}

/**
send_packet() is used to send packet to the client with data.
Type of packet sent here is mostly a data packet
@param PacketKind is the type of packet being send
@param seq_no is the sequence number of the packet
@param data is the data being sent by the server
*/
void RFTPServer::send_packet(Packet packet) {
	//Packet packet = Packet(pk, seq_no, size, data);
	uint8_t ptr[PACKET_SIZE];
	packet.serialize(ptr);
	sendto(this->sockS, ptr, PACKET_SIZE,0,(struct sockaddr *)&clientR,fromlen);
}
