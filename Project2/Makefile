main: serverS client #Builds everything

client: src/Client.cpp src/RFTPClient.cpp src/Packet.cpp
	g++ -g -w src/Client.cpp src/RFTPClient.cpp src/Packet.cpp -o client

serverS: src/Server.cpp src/RFTPServer.cpp src/Packet.cpp
	g++ -g -w src/Server.cpp src/RFTPServer.cpp src/Packet.cpp -o serverS

clean: #Cleans up executables
	rm client serverS ClientFileSystem/*
