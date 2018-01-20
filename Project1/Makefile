main: server client

client: src/Client.cpp
	g++ src/Client.cpp src/RFTPClient.cpp -o Client

server: src/Server.cpp
	g++ src/Server.cpp -o Server

clean: 
	rm Client Server
