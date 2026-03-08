#pragma once
#include <iostream>
#include "Header/Packet.h"

class Session;

class PacketHandler
{
public:
	//패킷 처리 함수
	static void HandlePacket( char* buffer, uint16_t packetSize);

private:
	//로직 처리 함수
	static void handleLogin(Packet& loginPacket);
	static void handleStartGame(Packet& mapPacket);
	static void handleSpawnUnit(Packet& unitSpawnPacket);
	static void handleMoveUnit(Packet& unitMovePacket);
};

