/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */
//Hello
#include "RFTPServer.h"

int main(int argc, char *argv[])
{
   if (argc != 1) {
      fprintf(stderr, "ERROR, no need of any command line arguments.\n");
      exit(0);
   }

   RFTPServer server;   
   server.Bind();
   server.receivePacket();
   return 0;
 }

