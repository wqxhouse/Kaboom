#pragma once

#include <WinSock2.h>

class ClientNetwork {
public:
    ClientNetwork();
    ~ClientNetwork();

    void connectToServer(const std::string &serverAddress, const int &serverPort);
	void disconnectFromServer();

    int receivePackets(char *);
    void sendMessage(char *message, int size);

	inline bool isConnected() const
	{
		return _connected;
	}

private:
    SOCKET clientSocket;
	bool _connected;
};