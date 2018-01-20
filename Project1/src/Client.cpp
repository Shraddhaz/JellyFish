/* UDP client in the internet domain */
#include "RFTPClient.h"

void error(const char *);
int main(int argc, char *argv[])
{
	char buffer[256];
	RFTPClient client;
	client.connectServer();
	printf("Please enter the message: ");
	client.sendData(buffer);
   	return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
