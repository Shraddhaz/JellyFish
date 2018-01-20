/* UDP client in the internet domain */
#include "RFTPClient.h"

void error(const char *);
int main(int argc, char *argv[])
{
	char buffer[256];
/**** This is now in RFTPClient class
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];
   
   if (argc != 1) { printf("No need of any arguments.\n");
                    exit(1);
   }

   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(HOSTNAME);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(PORT_NUMBER);
   length=sizeof(struct sockaddr_in);

   printf("Please enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
   n=sendto(sock,buffer,
            strlen(buffer),0,(const struct sockaddr *)&server,length);
   if (n < 0) error("Sendto");
   n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
   if (n < 0) error("recvfrom");
   write(1,"Got an ack: ",12);
   write(1,buffer,n);
   close(sock);
*/
	RFTPClient client;
	client.connectServer();
	 printf("Please enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
	client.sendData(buffer);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
