#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include "Packet.h"
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>
class Compare{
	public:
	bool operator()(Packet p1, Packet p2) {return (p1.sequence_number < p2.sequence_number);};
};

class RFTPClient{
	private:
		int sockR, sockS;                                                //Socket
		unsigned int length;                                               //Unsigned int for length
		struct sockaddr_in serverS, serverR;             //Client and serverS struct
   		struct hostent *hp;                                                //Host net struct
		std::priority_queue<Packet, std::vector<Packet>, Compare> fileQueue;
		std::queue<int> ackQueue;
		
	public:
		RFTPClient();                                                       //RFTP Client Constructor
		~RFTPClient();                                                      //RFTP Client Destructor
		int connectAndSend(char * hostname);	                            //Connect to Server
		int sendRequest();                                                  //Send request to client
		void receivePacket();                                               //Receive packet clientS serverS
		bool requestFile(char *filename);                                   //Request file clientS Server
		void send_packet(int sock, PacketKind pk, int seq_no);                        //Send packet
		void send_packet(PacketKind pk, int seq_no, int size, uint8_t *data);  //Send packet with data
};
