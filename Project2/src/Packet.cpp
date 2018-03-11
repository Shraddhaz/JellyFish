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
uint8_t* Packet:: serialize(){
	uint8_t buffer[PACKET_SIZE];
    memcpy(buffer, &(this->kind), sizeof(PacketKind));                     //Copying packet kind clientS packet to the buffer
    memcpy((buffer + SEQNO_LOC), &(this->sequence_number), sizeof(int));   //Copying sequence number clientS packet to buffer
    memcpy((buffer + DATA_SIZE_LOC), &(this->sizeOfData), sizeof(int));    //Copying size of data to send clientS packet to buffer
	memcpy((buffer + DATA_LOC), this->data, DATA_SIZE);                    //Copying data clientS packet to buffer
	return buffer;
}

/** Constructor for packet which is the deserializer that copies data
    from the buffer to the received packet
    @param buf is the buffer from which we deserialize
*/
Packet::Packet(uint8_t * buffer){
	memcpy(&(this->kind), buffer, sizeof(PacketKind));	
	memcpy(&(this->sequence_number), (buffer + SEQNO_LOC), sizeof(int));
	memcpy(&(this->sizeOfData), (buffer + DATA_SIZE_LOC), sizeof(int));
	memcpy(this->data, (buffer + DATA_LOC), DATA_SIZE);
}

/** Parametrized constructor for creating packet using the given parameters
    @param kind is the type of packet from the enum
    @param sequence_number is the sequence number of packet
    @param sizeOfData is the size of the data transferred
    @param pdata is the pointer to the transfered data
*/
Packet::Packet(PacketKind kind,int sequence_number , int sizeOfData, uint8_t *pdata){
	this->kind = kind;
	this->sequence_number = sequence_number;
	this->sizeOfData = sizeOfData;
	memcpy(this->data, pdata, DATA_SIZE);
}

//Function to display packet elements like the kind and sequence number
void Packet::printPacket()
{
	cout<<"****************** Printing Packet *********************"<<endl;
	cout<<"Kind :"<<this->kind<<endl;;                                      //Print the packet type
	cout<<"Sequence number"<<this->sequence_number<<endl;                   //Print the sequence number
}

Packet::Packet(const Packet &pack){
	this->kind = pack.kind;
	//this->data = malloc(DATA_SIZE);
	//memcpy(this->data, pack.data, DATA_SIZE);
	this->sequence_number = pack.sequence_number; 
	this->sizeOfData = pack.sizeOfData;
}
//Destructor for packet that deallocates memory allocated to data
Packet::~Packet()
{
}
