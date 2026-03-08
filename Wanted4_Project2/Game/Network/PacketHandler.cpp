#include "PacketHandler.h"
#include "Session.h"
#include "Level/Game.h"
#include "Actor/Npc.h"


void PacketHandler::HandlePacket(char* buffer, uint16_t packetSize)
{
	Packet Packet;

	Packet.getBuffer().assign(buffer, buffer + packetSize);
	uint16_t totalSize = Packet.read<uint16_t>();   // 전체 크기
	uint16_t packetId = Packet.read<uint16_t>();   // 패킷 타임

	switch (packetId)
	{
	case PK_SC_WAITING:
	{
		handleLogin(Packet);
		break;
	}
	case PK_SC_GAME_START:
	{
		handleStartGame(Packet);
		break;
	}
	case PK_SC_SPAWN_UNIT:
	{
		handleSpawnUnit(Packet);
		break;
	}
	case PK_SC_MOVE_UNIT:
	{
		handleMoveUnit(Packet);
		break;
	}
	}
}

void PacketHandler::handleLogin (Packet& loginPacket)
{
	uint16_t playerId = loginPacket.read<uint16_t>();
	Game::Get().WaitingRoom(playerId);
}

void PacketHandler::handleStartGame(Packet& mapPacket)
{
	uint16_t width = mapPacket.read<uint16_t>();   // 맵 너비
	uint16_t height = mapPacket.read<uint16_t>();   // 맵 높이

	// 비트 packed 맵 언팩
	int totalTiles = width * height;
	int packedSize = (totalTiles + 7) / 8;

	std::vector<uint8_t> packedData(packedSize);
	for (int i = 0; i < packedSize; ++i)
	{
		packedData[i] = mapPacket.read<uint8_t>();
	}

	std::vector<uint8_t> tiles(totalTiles, 0);
	for (int i = 0; i < totalTiles; ++i)
	{
		int byteIdx = i / 8;
		int bitIdx = i % 8;
		if (packedData[byteIdx] & (1 << bitIdx))
			tiles[i] = 1;
	}

	Game::Get().GameStart(width, height, std::move(tiles));
}

void PacketHandler::handleSpawnUnit(Packet& unitSpawnPacket)
{
	uint16_t unitId = unitSpawnPacket.read<uint16_t>();
	uint16_t playerId = unitSpawnPacket.read<uint16_t>();
	Vector2 spawnPos = unitSpawnPacket.read<Vector2>();

	Game::Get().UnitSpawn(unitId, playerId, spawnPos);
}

void PacketHandler::handleMoveUnit(Packet& unitMovePacket)
{
	uint16_t unitId = unitMovePacket.read<uint16_t>();
	uint16_t playerId = unitMovePacket.read<uint16_t>();
	uint16_t pathCount = unitMovePacket.read<uint16_t>(); // 경로 개수 먼저 읽기

	std::vector<Vector2> path;
	for (int i = 0; i < pathCount; i++)
	{
		Vector2 pos = unitMovePacket.read<Vector2>();
		path.push_back(pos);
	}

	//Todo 유닛 이동 로직 작성

}
