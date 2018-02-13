/**Packet.cpp has all the packet functions implementation like
Packet constructor, destructor, serialize, deserialize, display packet */
#include "Packet.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

/** Serializing the packet for data transfer by allocating memory
    for packet and copying from packet to buffer
    @return buffer used for file transfer
*/
void* Packet:: serialize(){
	void *buf = malloc(PACKET_SIZE);                                    //Allocating memory for packet
    memcpy(buf, &(this->kind), sizeof(PacketKind));                     //Copying packet kind from packet to the buffer
    memcpy((buf + SEQNO_LOC), &(this->sequence_number), sizeof(int));   //Copying sequence number from packet to buffer
    memcpy((buf + DATA_SIZE_LOC), &(this->sizeOfData), sizeof(int));    //Copying size of data to send from packet to buffer
	memcpy((buf + DATA_LOC), this->data, DATA_SIZE);                    //Copying data from packet to buffer
	return buf;
}

/** Constructor for packet which is the deserializer that copies data
    from the buffer to the received packet
    @param buf is the buffer from which we deserialize
*/
Packet::Packet(void *buf){
	memcpy(&(this->kind), buf, sizeof(PacketKind));	
	memcpy(&(this->sequence_number), (buf + SEQNO_LOC), sizeof(int));
	memcpy(&(this->sizeOfData), (buf + DATA_SIZE_LOC), sizeof(int));
	this->data = malloc(DATA_SIZE);
	memcpy(this->data, (buf + DATA_LOC), DATA_SIZE);
}

/** Parametrized constructor for creating packet using the given parameters
    @param kind is the type of packet from the enum
    @param sequence_number is the sequence number of packet
    @param sizeOfData is the size of the data transferred
    @param pdata is the pointer to the transfered data
*/
Packet::Packet(PacketKind kind,int sequence_number , int sizeOfData, void *pdata){
	this->kind = kind;
	this->sequence_number = sequence_number;
	this->sizeOfData = sizeOfData;
	this->data = malloc(DATA_SIZE);
	memcpy(this->data, pdata, DATA_SIZE);
}

//Function to display packet elements like the kind and sequence number
void Packet::printPacket()
{
	cout<<"****************** Printing Packet *********************"<<endl;
	cout<<"Kind :"<<this->kind<<endl;;                                      //Print the packet type
	cout<<"Sequence number"<<this->sequence_number<<endl;                   //Print the sequence number
}

//Destructor for packet that deallocates memory allocated to data
Packet::~Packet()
{
	free(this->data);                                                       //Free the memory allocated for data
}
