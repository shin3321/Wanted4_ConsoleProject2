#pragma once
//===============================
//			Session
// 오직 네트워크 기능만 하는 클래스
//===============================
class Player;

class Session
{
public:
	Session(OverlappedExPool& pool);
	

	void doSend(const char* packet, uint16_t packetSize);
	void doRecv();

	OverlappedEx* _recvOver;
	OverlappedEx* _sendOver;

	uint16_t _recvBytes = 0;
	SOCKET _socket;
	int _id;

private:
	std::shared_ptr<Player> _player;
	OverlappedExPool* _overlappedPool;

};