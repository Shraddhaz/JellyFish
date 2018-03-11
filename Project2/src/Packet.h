/** Packet.h has the declaration of the packet class and its methods
*/
#include "Constants.h"

//Creating Packet class
class Packet {
	public:
		PacketKind kind;                                        //Type of packet
		int sequence_number;                                    //Sequence number of the packet
		int sizeOfData;                                         //Size of Data being transferred
		uint8_t data[DATA_SIZE];                                             //Pointer to the data being transferred
		
	public:
		uint8_t* serialize(uint8_t* );                                      //Serializer
		Packet(uint8_t *);                                         //Constructor which is the Deserializer
		Packet(const Packet &obj);                                         //Constructor which is the Deserializer
		Packet();                                         //Constructor which is the Deserializer
		Packet(PacketKind pk,int sn, int size, uint8_t *data);     //Parameterized constructor
		~Packet();	                                            //Destructor for Packet class
		void printPacket();                                     //Printing packet elements
};
