#include "pch.h"
#include "PacketHandler.h"
#include "01.Game/Game.h"
#include "01.Game/Actor/Player.h"
#include "Packet.h"


void PacketHandler::handlePacket(char* buffer, uint16_t id)
{
	uint16 type = buffer[2];

	switch (type)
	{
	case PK_CS_ID:
	{
		handleLogin(buffer, id);
		break;
	}
	}

}

void PacketHandler::handleLogin(char* packet, uint16_t id)
{
	uint16_t offset = 0;
	//사이즈와 타입 ㄷ더하기	

	offset += sizeof(uint16_t) * 2;
	uint16_t nickNameSize = sizeof(packet) - (sizeof(uint16_t) * 2);
	std::string nickName(reinterpret_cast<char*>(&packet[offset]), nickNameSize);

	Game::get().setPlayer(std::make_shared<Player>(nickName), id);
	Game::get().waitingRoom(id);
}
