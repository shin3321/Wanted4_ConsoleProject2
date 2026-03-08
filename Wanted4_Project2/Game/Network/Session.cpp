#include "Session.h"
#include "Header/Packet.h"
#include "Network/PacketHandler.h"
#include "Actor/Player.h"
#include <iostream>


Session::Session(SOCKET socket)
	:_socket(socket), _ringBuffer(BUFSIZE)
{
}

Session::~Session()
{
}


void Session::DoSend(void* packet, uint16_t packetSize)
{
	int result = ::send(_socket, (const char*)packet, packetSize, 0);
	if (result == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		//WSAEWOULDBLOCK는 정상임
		if (err != WSAEWOULDBLOCK)
		{
			std::cout << "Send Error: " << err;
		}
	}
}

void Session::DoRecv()
{
	while (true)
	{
		auto [ptr, size] = _ringBuffer.getWritePtr();
		if (size == 0) return;

		int result = ::recv(_socket, ptr, (int)size, 0);
		if (result > 0)
		{
			_ringBuffer.onWrite(result);
			while (true)
			{
				//받은 파일의 크기가 헤더의 크기보다 작으면 실패
				if (_ringBuffer.size() < sizeof(uint16_t))
					break;
				uint16_t packetSize = 0;
				_ringBuffer.peek(reinterpret_cast<char*>(&packetSize), sizeof(uint16_t));
				packetSize = ntohs(packetSize);

				if (packetSize == 0)break;
				if (_ringBuffer.size() < packetSize)
				{
					std::cout << "Invalid Packet size: " << packetSize;
					break;
				}
				std::vector<char> packetData(packetSize);
				_ringBuffer.read(packetData.data(), packetSize); // read가 내부에서 head를 옮김

				PacketHandler::HandlePacket(packetData.data(), packetSize);
			}
		}
		else if (result == 0)
		{
			std::cout << "Disconnected bt sever\n";
			break;
		}
		else
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
			{
				break;
			}
			else
			{
				printf("recv error: %d\n", err);
				break;
			}
		}
	}
}

void Session::SetMyPlayer(Player* player, uint16_t id)
{
	_myPlayer = player;
	_id = _myPlayer->GetId();
	_myPlayer->SetSender(this); 
}
