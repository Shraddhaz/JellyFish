/*UDP client in the internet domain */
#include "RFTPClient.h"
#include <iostream>

using namespace std;

void error(const char *);
int main(int argc, char *argv[])
{
	char buffer[256];
	RFTPClient client;
	if(client.connectServer()) 
		cout<<"Connect server done!\n";
	cout<<"Please enter the message: "<<endl;
	client.sendData(buffer);
   	return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
