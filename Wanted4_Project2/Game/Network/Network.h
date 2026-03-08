#pragma once

#include <iostream>

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include <mswsock.h>
#include "Header/Packet.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

class Network
{
public:
	Network();

	void Init();
	void DoRecv();
	void ShutDown();

	Network& get();
public:
	HANDLE _iocpHandle = nullptr;
	SOCKET _socket = INVALID_SOCKET;

private:
	static Network* _instance;

	const char* serverIp = "127.0.0.1";
	bool _isRunning = false;
};

