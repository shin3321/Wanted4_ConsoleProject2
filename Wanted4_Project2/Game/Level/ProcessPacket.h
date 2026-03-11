#pragma once
#include "Level\Level.h"
#include "Header/Packet.h"
#include "Level/Game.h"
#include "Network/PacketHandler.h"
#include "Network/PacketQueue.h"

#include <mutex>
#include <queue>
#include <algorithm>

using namespace Wanted;
class ProcessPacket :   public Level
{
	RTTI_DECLARATIONS(ProcessPacket, Level)


public:
	ProcessPacket()
	{}

	~ProcessPacket() {}

	void ShutDown()
	{
		_isRunnig = false;
	}

	void ProcessPackets()
	{
		while(_isRunnig){
			std::queue<std::shared_ptr<Packet>>_localQueue;
			{
				std::lock_guard<std::mutex> lock(PacketQueue::Get().GetMutex());
				std::swap(PacketQueue::Get().GetQueue(), _localQueue);
			}
			while (!_localQueue.empty()) {
				PacketHandler::HandlePacket(*_localQueue.front());
				_localQueue.pop();
			}
		}
	}

private:
	bool _isRunnig = true;
};

