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
class ManageLevel :   public Level
{
	RTTI_DECLARATIONS(ManageLevel, Level)


public:
	ManageLevel()
	{}

	~ManageLevel() {}

	void Tick(float deltaTime) override
	{
		ProcessPackets();  // 패킷 처리 공통화
		super::Tick(deltaTime);
	}

	void ProcessPackets()
	{
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
};

