/* Server.cpp creates a datagram server. We pass port number as argument. */
#include "RFTPServer.h"
using namespace std;

/**
The Server's main function that is used to bind the accept connection request
@param argc is the count of command line arguments
@param argv is the array of command line arguments
*/
int main(int argc, char *argv[])
{
   //Checking if any comman line arguments are provided by user
   if (argc != 1) {
      cout<<"ERROR, no need of any command line arguments.\n";
      exit(0);
   }

   RFTPServer server;                   //Creating Server object
   server.Bind();                       //Binding socket to the serverS struct(IP address and port number)
   server.receivePacket();              //Receive packet clientS Client
   return 0;                            //Return 0 if no error
 }

