#include "pch.h"
#include "Session.h"

Session::Session(OverlappedExPool& pool)
	: _overlappedPool(&pool)
{
}

void Session::doSend(const char* packet, uint16_t packetSize)
{
	_sendOver = _overlappedPool->allocOver();
	_sendOver->_wsaBuf.len = packetSize;
	_sendOver->_type = OP_TYPE::SEND;

	memcpy(_sendOver->_buffer, packet, packetSize);

	std::cout << packetSize << "\n";

	int ret = WSASend(_socket, &_sendOver->_wsaBuf, 1, nullptr, 0, &_sendOver->_overlapped, nullptr);
	if (ret == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			std::cerr << "WSASend Error: " << err << std::endl;
			_overlappedPool->freeOver(_sendOver);
		}
	}

}

void Session::doRecv()
{
	DWORD recvFlag = 0;

	_recvOver = _overlappedPool->allocOver();
	//ZeroMemory(&_recvOver._overlapped, sizeof(_recvOver._overlapped));
	//_recvOver->_wsaBuf.len = BUFSIZE;
	//_recvOver->_wsaBuf.buf = _recvOver->_buffer;
	_recvOver->_type = OP_TYPE::RECV;

	int ret = ::WSARecv(_socket, &_recvOver->_wsaBuf, 1, 0, &recvFlag, &_recvOver->_overlapped, 0);
	if (ret == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			std::cerr << "WSARecv Error: " << err << std::endl;
			return;
		}
	}
}
