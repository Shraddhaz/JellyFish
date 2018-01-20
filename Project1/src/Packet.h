#include "Constants.h"

class Packet {
	private:
		PacketKind kind;
		bool isClient;
		int clientID;
		void *data;			
		/*
		 * I strongly believe we will need multiple
		 * classes for this. Because for data we will
		 * need void * which is useless in all other cases.
		 */
};
