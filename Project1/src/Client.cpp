/*UDP client in the internet domain */
#include "RFTPClient.h"
#include <iostream>

using namespace std;

void error(const char *);
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout<<"Usage: ./Client <filename>"<<endl;
		return 0;
	}
	char buffer[256];
	RFTPClient client;
	if(client.connectAndSend()) 
		cout<<"Connection Request Sent\n";
   	cout<<"Requesting file: "<<argv[1]<<endl;
	bool b = client.requestFile(argv[1]);
	return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
