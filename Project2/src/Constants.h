/* Constants.h has all the default constants used throughout the programs */
#include <netinet/in.h>

#define PORT_NUMBER_DATA 2939	//Default port for sender
#define PORT_NUMBER_ACK 1613	//Default port for receiver
#define DATA_SIZE 1024      //Default size of data sent
#define TIMEOUT 10
//Enum for various types of packets with PacketKind as its object
typedef enum {
	CONNECTION_REQUEST,                             //Connection Request
	CONNECTION_ACK,                                 //Connection Acknowledgement
	FILE_REQUEST,                                   //File Request
	FILE_REQUEST_ACK,                               //File Request Acknowledgement
	FILE_REQ_ERROR,                                 //File Request Error
	START_DATA_TRANSFER,                            //Begin File Transfer
	DATA,                                           //Data only
	DATA_ACK,                                       //Data Acknowledgement
	CLOSE_CONNECTION,                               //Close Connection
	NONE
} PacketKind;


//Size of packet elements
#define SEQNO_LOC sizeof(PacketKind)                //Default size of packet type
#define DATA_SIZE_LOC (SEQNO_LOC + sizeof(int))     //Default size of sequence number
#define DATA_LOC (DATA_SIZE_LOC + sizeof(int))      //Default size of data

#define PACKET_SIZE (sizeof(PacketKind) + sizeof(int) + sizeof(int) + DATA_SIZE)    //Default packet size

#define client_fs "ClientFileSystem"                //Alias for ClientFileSystem
#define server_fs "ServerFileSystem"                //Alias for ServerFileSystem
#define MAX_WINDOW_SIZE 25
