main: server client #Builds everything

client: src/Client.cpp src/RFTPClient.cpp src/Packet.cpp
	g++ -g -w src/Client.cpp src/RFTPClient.cpp src/Packet.cpp -o client

server: src/Server.cpp src/RFTPServer.cpp src/Packet.cpp
	g++ -g -w src/Server.cpp src/RFTPServer.cpp src/Packet.cpp -o server

clean: #Cleans up executables
	rm client server ClientFileSystem/*
