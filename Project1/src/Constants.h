#define PORT_NUMBER 2939	//This will be our default port for our protocol.
#define HOSTNAME "ada" //Because we are always going to run on linux lab machines.

#define DATA_SIZE 1024

typedef enum {
	CONNECTION_REQUEST,
	CONNECTION_ACK,
	FILE_REQUEST,
	FILE_REQUEST_ACK,
	DATA,
	DATA_ACK,
	CLOSE_CONNECTION,
} PacketKind;


#define SEQNO_LOC sizeof(PacketKind)
#define DATA_LOC (SEQNO_LOC + sizeof(int)) 

#define PACKET_SIZE (sizeof(PacketKind) + sizeof(int) + DATA_SIZE)
