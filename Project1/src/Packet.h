#include "Constants.h"

class Packet {
	public:
		PacketKind kind;
		int sequence_number;
		void *data;
		
	public:
		void* serialize();
		Packet(void *);
		Packet(PacketKind pk,int sn, void *data);		
		void printPacket();
};
