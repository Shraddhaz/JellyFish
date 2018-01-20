main: server client #Builds everything

client: src/Client.cpp src/RFTPClient.cpp #Builds client
	g++ -g src/Client.cpp src/RFTPClient.cpp -o Client

server: src/Server.cpp #Builds Server
	g++ -g src/Server.cpp -o Server

clean: #Cleans up executables
	rm Client Server
