#pragma once
#include <mutex>
#include <queue>

#include "Header/Packet.h"

class PacketQueue
{
public:
	PacketQueue();
	~PacketQueue();
	std::queue<std::shared_ptr<Packet>>& GetQueue() { return _packetqueue; }
	std::mutex& GetMutex() { return _queueLock; }

	void PushQueue(std::shared_ptr<Packet> packet);
	std::shared_ptr<Packet> PopQueue();

	//싱글톤을 위한 겟 함수
	static PacketQueue& Get();

private:
	//static PacketQueue* instance;
	std::queue<std::shared_ptr<Packet>> _packetqueue;
	std::mutex _queueLock;
};

