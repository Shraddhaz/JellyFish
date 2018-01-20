#include "RFTPServer.h"

RFTPServer::RFTPServer(){
	sock=socket(AF_INET, SOCK_DGRAM, 0);
   	
	if (sock < 0) error("Opening socket");
   		length = sizeof(server);
   	bzero(&server,length);
   	
	server.sin_family=AF_INET;
   	server.sin_addr.s_addr=INADDR_ANY;
   	server.sin_port=htons(PORT_NUMBER);
}

RFTPServer::Bind(){
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
}

RFTPServer::ListenAccept{
   while (1) {
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
   }
   return 0;
}
