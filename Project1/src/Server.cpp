/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */

#include "RFTPServer.h"

int main(int argc, char *argv[])
{
   if (argc != 1) {
      fprintf(stderr, "ERROR, no need of any command line arguments.\n");
      exit(0);
   }

   RFTPServer server;   
   server.Bind();
   server.ListenAccept();
   return 0;
 }

