#pragma once
#include <iostream>
#include "Header/Packet.h"

class Session;

class PacketHandler
{
public:
	//패킷 처리 함수
	static void HandlePacket(Packet& packet);

private:
	//로직 처리 함수
	static void handleLogin(Packet& loginPacket);
	static void handleStartGame(Packet& mapPacket);

	static void handleSpawnUnit(Packet& unitSpawnPacket);
	static void handleMoveUnit(Packet& unitMovePacket);
	static void handleAttackedUnit(Packet& unitAttackedPacket);
	static void handleDespawnUnit(Packet& unitDespawnPacket);

	static void handleConstructCastle (Packet& castlePacket);
	static void handleAttackedCastle (Packet& castlePacket);
	static void handleDestroyCastle (Packet& castlePacket);
};

