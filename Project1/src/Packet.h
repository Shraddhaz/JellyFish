#include "Constants.h"

class Packet {
	private:
		PacketKind kind;
		int sequence_number;
		void *data;
		
	public:
		void* serialize();
		Packet(void *);
		Packet(PacketKind,int, void *);		
};
