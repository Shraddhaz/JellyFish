#include "Packet.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

void* Packet:: serialize(){
	void *buf = malloc(PACKET_SIZE);
    memcpy(buf, &(this->kind), sizeof(PacketKind));
    memcpy((buf + SEQNO_LOC), &(this->sequence_number), sizeof(int));
    memcpy((buf + DATA_SIZE_LOC), &(this->sizeOfData), sizeof(int));
	memcpy((buf + DATA_LOC), this->data, DATA_SIZE);
	return buf;
}

Packet::Packet(void *buf){
	memcpy(&(this->kind), buf, sizeof(PacketKind));	
	memcpy(&(this->sequence_number), (buf + SEQNO_LOC), sizeof(int));
	memcpy(&(this->sizeOfData), (buf + DATA_SIZE_LOC), sizeof(int));
	this->data = malloc(DATA_SIZE);
	memcpy(this->data, (buf + DATA_LOC), DATA_SIZE);
}

Packet::Packet(PacketKind kind,int sequence_number , int sizeOfData, void *pdata){
	this->kind = kind;
	this->sequence_number = sequence_number;
	this->sizeOfData = sizeOfData;
	this->data = malloc(DATA_SIZE);
	memcpy(this->data, pdata, DATA_SIZE);
}

void Packet::printPacket()
{
	cout<<"****************** Printing Packet *********************"<<endl;
	cout<<"Kind :"<<this->kind<<endl;;
	cout<<"Sequence number"<<this->sequence_number<<endl;
}

Packet::~Packet()
{
//	free(this->data);
}
