#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include "Packet.h"
#include <iostream>
#include <fcntl.h>
class RFTPServer {
	private:
		int sock, length;
   		socklen_t fromlen;
   		struct sockaddr_in server;
   		struct sockaddr_in from;
		bool isConnected;
	public:
		RFTPServer();
		//~RFTPServer();
		void Bind();
		void ListenAccept();
		void receivePacket();
		void fileReq(void * ptr, int size);
		bool send_packet(PacketKind pk, int sn);
		bool send_packet(PacketKind pk, int sn, int size, void *data);
};
