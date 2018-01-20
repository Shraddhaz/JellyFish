main: server client #Builds everything

client: src/Client.cpp src/RFTPClient.cpp #Builds client
	g++ src/Client.cpp src/RFTPClient.cpp -o Client

server: src/Server.cpp
	g++ src/Server.cpp -o Server

clean: 
	rm Client Server
