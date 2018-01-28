#include "Constants.h"

typedef struct Packet {
		PacketKind kind;
		int sequence_number;
		void *data;		
}Packet;
