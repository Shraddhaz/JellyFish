#include "Packet.h"

void* Packet:: serialize(){

}

Packet::Packet(void *buf){

}

Packet::Packet(PacketKind kind,int sequence_number , void *pdata){
	this.kind = kind;
	this.sequence_number = sequence_number;
	this.data = pdata;
}
