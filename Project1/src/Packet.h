#include "Constants.h"

class Packet {
	public:
		PacketKind kind;
		int sequence_number;
		int sizeOfData;
		void *data;
		
	public:
		void* serialize();
		Packet(void *);
		Packet(PacketKind pk,int sn, int size, void *data);
		~Packet();	
		void printPacket();
};
