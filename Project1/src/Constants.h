#define PORT_NUMBER 2939	//This will be our default port for our protocol.
#define HOSTNAME "127.0.0.1" //Because we are always going to run on linux lab machines.

#define DATA_SIZE 1024

typedef enum {
	CONNECTION_REQUEST,
	CONNECTION_ACK,
	FILE_REQUEST,
	FILE_REQUEST_ACK,
	START_DATA_TRANSFER,
	DATA,
	DATA_ACK,
	CLOSE_CONNECTION,
} PacketKind;


#define SEQNO_LOC sizeof(PacketKind)
#define DATA_SIZE_LOC (SEQNO_LOC + sizeof(int))
#define DATA_LOC (DATA_SIZE_LOC + sizeof(int)) 

#define PACKET_SIZE (sizeof(PacketKind) + sizeof(int) + sizeof(int) + DATA_SIZE)

#define client_fs "ClientFileSystem"
#define server_fs "ServerFileSystem"
