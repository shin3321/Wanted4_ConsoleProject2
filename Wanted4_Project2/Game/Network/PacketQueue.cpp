#include "PacketQueue.h"

//PacketQueue* PacketQueue::instance = nullptr;

PacketQueue::PacketQueue()
{
}

PacketQueue::~PacketQueue()
{
}

void PacketQueue::PushQueue(std::shared_ptr<Packet> packet)
{
	std::lock_guard<std::mutex>lock(_queueLock);
	_packetqueue.push(std::move(packet));
}

std::shared_ptr<Packet> PacketQueue::PopQueue()
{
	std::lock_guard<std::mutex>lock(_queueLock);
	if (_packetqueue.empty()) return {};
	std::shared_ptr<Packet> packet = std::move(_packetqueue.front());
	_packetqueue.pop();
	return packet;
}

PacketQueue& PacketQueue::Get()
{
	static PacketQueue instance;
	return instance;
	// TODO: insert return statement here
}
