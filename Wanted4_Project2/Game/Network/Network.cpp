#include "Network.h"
#include "Level/Game.h"
#include "Network/Session.h"
constexpr short GAME_PORT = 9999;

Network* Network::_instance = nullptr;
Network::Network()
{
	_instance = this;
}

void Network::Init()
{
	_isRunning = true;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return;
	}

	_socket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, 0);
	if (_socket == INVALID_SOCKET) return;

	//클라이언트 소켓 논블로킹으로 바꾸기
	u_long on = 1;
	ioctlsocket(_socket, FIONBIO, &on);

	//서버의 포트 번호 ip 연결
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(GAME_PORT);

	//서버와 연결
	if (::connect(_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)))
	{
		//오류 처리
		int err = WSAGetLastError();
		if (err == WSAEINPROGRESS || err == WSAEALREADY)
		{
			return;
		}
	}
	Game::Get().SetSession(std::make_shared<Session>(_socket));
	DoRecv();
}

void Network::DoRecv()
{
	while (_isRunning)
	{
		Game::Get().getMySesseion()->DoRecv();
	}
}

void Network::ShutDown()
{
	if (_isRunning)
		_isRunning = false;
	if (_socket != INVALID_SOCKET)
	{
		closesocket(_socket);
		_socket = INVALID_SOCKET;
	}
}

Network& Network::get()
{
	if (!_instance)
	{
		std::cout << "instance is null";
		__debugbreak();
	}
	return *_instance;
	// TODO: insert return statement here
}
