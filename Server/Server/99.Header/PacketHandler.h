#pragma once
class PacketHandler
{
public:
	//패킷 처리 함수
	static void handlePacket(char* buffer, uint16_t id);

private:
	static void handleLogin(char* packet, uint16_t id);
};

