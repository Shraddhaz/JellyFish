/**
RFTPServer.h has the class RFTPServer which holds all the data members
used by the Server for its implementation and all the server methods
like binding, listening for requests, accepting requests, sending and
receiving packets
*/

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include "Packet.h"
//#include "Packet.cpp"
#include <iostream>
#include <fcntl.h>
#include <queue>
#include <chrono>
#include <unordered_map>

using namespace std;
/*
typedef enum{
	SEND,
	RESEND
} Status;

typedef struct PacketWrap{
	Packet pack;
	Status s;
	chrono::time_point<chrono::system_clock> time;
} PWrap;
*/
typedef enum{
    SEND,
    RESEND
} Status;

typedef struct PacketWrap{
    Packet *pack;
    Status s;
    chrono::time_point<chrono::system_clock> time;
} PWrap;

//PWrap packetWrap;

class RFTPServer {
    public:
        pthread_mutex_t lock;
		pthread_cond_t isEmpty;
		pthread_cond_t isFull;
		unordered_map<int, PWrap> packetMap;

		int sockS, sockR, length, lengthAck;
		int fdRead;
   		socklen_t fromlen;                                         //Client length
   		struct sockaddr_in serverS;                                              //Server's socket struct
		struct sockaddr_in serverR;
   		struct sockaddr_in clientS;                                                //Client's socket struct
		struct sockaddr_in clientR;
		bool isConnected;                                                       //Connection established or not
		struct timeval read_timeout;                                            //Timeout for reading data
		struct timeval reset_timeout;                                           //Timeout after reset


		RFTPServer();                                                           //RFTPServer Constructor
		void Bind();                                                            //Binding Server socket to Portno and IP
		void ListenAccept();                                                    //Listen to request and accept
		void receivePacket();                                                   //Receive packet and deserialize it
		bool fileReq(uint8_t *ptr, int size);                                     //Handle retransmission and timeout
		void send_packet(PacketKind pk, int sn);                                //Send packet
		void send_packet(Packet p);          //Send packet with data
};

void* receiver(void* rcvargs);
