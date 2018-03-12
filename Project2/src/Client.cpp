//Client.cpp has the client implementation for the file request-transfer
#include "RFTPClient.h"
using namespace std;

/**
The Client's main function that is used to connect to the server and request for the file
@param argc is the count of command line parameters
@param argv is the array of command line parameters
*/
int main(int argc, char *argv[])
{
    //Checking if command line arguments are provided by user
	if (argc != 3)
	{
		cout<<"Usage: ./Client <host> <filename>"<<endl;
		return 0;
	}

	//Creating buffer for file transfer
	char buffer[256];

    //Creating client object
	RFTPClient client;

	//Establishing connection with serverS
	if(client.connectAndSend(argv[1]))
		cout<<"Connection Request Sent\n";
	else
		cout<<"Error establishing connection request.\n";

    //Requesting file
    cout<<"Requesting file: "<<argv[2]<<endl;
	bool b = client.requestFile(argv[2]);

	//Checking if file transfer is successful
	if (b)
		cout<<"File transfer complete.\n";
	else
		cout<<"Error transferring the file.\n"
				"Check filename entered and/or check you connectivity.\n";

	return 0;
}
