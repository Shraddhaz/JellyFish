/** Packet.h has the declaration of the packet class and its methods
*/
#include "Constants.h"

//Creating Packet class
class Packet {
	public:
		PacketKind kind;                                        //Type of packet
		int sequence_number;                                    //Sequence number of the packet
		int sizeOfData;                                         //Size of Data being transferred
		void *data;                                             //Pointer to the data being transferred
		
	public:
		void* serialize();                                      //Serializer
		Packet(void *);                                         //Constructor which is the Deserializer
		Packet(PacketKind pk,int sn, int size, void *data);     //Parameterized constructor
		~Packet();	                                            //Destructor for Packet class
		void printPacket();                                     //Printing packet elements
};
