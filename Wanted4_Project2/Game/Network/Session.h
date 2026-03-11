#pragma once
#include "Network/INetwork.h"
#include "Header/Packet.h"
#include "Header/RingBuffer.h"
#include <winsock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <memory> 

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

class Player;

class Session : public INetwork
{
public:
	Session(SOCKET socket);
	~Session();

public:
	virtual void DoSend(void* packet, uint16_t packetSize)override;
	virtual void DoRecv() override;

	//setter
	void SetMyPlayer(std::shared_ptr<Player> player, uint16_t id);

	//getter
	int GetId() { return _id; }
	std::shared_ptr<Player> GetMyPlayer() { return _myPlayer; }

private:
	int _id;
	SOCKET _socket;

	RingBuffer _ringBuffer;
	char _recvBuf[BUFSIZE];
	char _sendBuf[BUFSIZE];

	size_t _remainSize = 0;

private:
	std::shared_ptr<Player> _myPlayer;
};
