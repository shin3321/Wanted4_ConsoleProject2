#pragma once

enum class OP_TYPE
{
	ACCEPT,
	RECV,
	SEND,
	GAME_START,

};

//OVERLAPPED 확장 구조체
struct OverlappedEx
{
	WSAOVERLAPPED _overlapped;
	// 데이터 버퍼
	WSABUF _wsaBuf;
	char _buffer[BUFSIZE] = {};
	OP_TYPE _type;

	//recv용 생성자
	OverlappedEx()
	{
		_wsaBuf.len = BUFSIZE;
		_wsaBuf.buf = _buffer;
		ZeroMemory(&_overlapped, sizeof(_overlapped));
		_type = OP_TYPE::RECV;
	}

	//send용 생성자
	OverlappedEx(char* packet, size_t packetSize)
	{
		_wsaBuf.len = BUFSIZE;
		_wsaBuf.buf = _buffer;
		ZeroMemory(&_overlapped, sizeof(_overlapped));
		_type = OP_TYPE::SEND;

		memcpy(_buffer, packet, packetSize);
	}
};

class OverlappedExPool
{
public:
	//생성
	OverlappedExPool(int size)
	{
		for (int i = 0; i < size; ++i)
		{
			_pool.push(new OverlappedEx());
		}
	}

	//할당
	OverlappedEx* allocOver()
	{
		//pool이 비어 있으면 재할당
		if (_pool.empty())
		{
			return new OverlappedEx();
		}

		OverlappedEx* overlapped = _pool.front();
		_pool.pop();
		return overlapped;
	}

	//반납
	void freeOver(OverlappedEx* overlapped)
	{
		if (overlapped)
		{
			ZeroMemory(&overlapped, sizeof(OverlappedEx));
			std::lock_guard<std::mutex> lock(_poolMutex);
			_pool.push(overlapped);
		}
	}

private:
	std::mutex _poolMutex;
	std::queue<OverlappedEx*> _pool;
};