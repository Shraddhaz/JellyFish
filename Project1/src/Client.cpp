/*UDP client in the internet domain */
#include "RFTPClient.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout<<"Usage: ./Client <host> <filename>"<<endl;
		return 0;
	}
	char buffer[256];
	RFTPClient client;
	if(client.connectAndSend(argv[1])) 
		cout<<"Connection Request Sent\n";
	else
		cout<<"Error establishing connection request.\n";
   	cout<<"Requesting file: "<<argv[2]<<endl;
	bool b = client.requestFile(argv[2]);
	if (b)
		cout<<"File transfer oomplete.\n";
	else
		cout<<"Error transferring the file.\n"
				"Check filename entered and/or check you connectivity.\n";
	return 0;
}
