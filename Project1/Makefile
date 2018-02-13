main: server client #Builds everything

client: src/Client.cpp src/RFTPClient.cpp src/Packet.cpp
	g++ -g -w src/Client.cpp src/RFTPClient.cpp src/Packet.cpp -o Client

server: src/Server.cpp src/RFTPServer.cpp src/Packet.cpp
	g++ -g -w src/Server.cpp src/RFTPServer.cpp src/Packet.cpp -o Server

clean: #Cleans up executables
	rm Client Server ClientFileSystem/*
