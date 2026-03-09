#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Game.h"

#include "Level/WaitingRoomLevel.h"
#include "Level/LoginLevel.h"
#include "Level/GameLevel.h"
#include "Header/Packet.h"
#include "Actor/Player.h"
#include "Actor/Unit.h"
#include "Network/Session.h"

#include <thread>
#include <iostream>

Game* Game::instance = nullptr;
Game::Game()
{
	instance = this;

	levels.emplace_back(new LoginLevel());
	state = State::Login;
	mainLevel = levels[0];
}

Game::~Game()
{
	if (_myPlayer)
	{
		SafeDelete(_myPlayer);
		_myPlayer = nullptr;
	}
}

void Game::GameStart(uint16_t width, uint16_t height, std::vector<uint8_t> tiles)
{
	system("cls");

	//mainlevel 변경
	mainLevel = new GameLevel(width, height, std::move(tiles));
	_myPlayer = new Player(_myId);
	mySession->SetMyPlayer(_myPlayer, _myId);
	mainLevel->AddNewActor(_myPlayer);
}

void Game::WaitingRoom(uint16_t playerId)
{
	system("cls");

	//mainlevel 변경
	mainLevel = new WaitingRoomLevel();
	_myId = playerId;
}

void Game::SetSession(const std::shared_ptr<Session> session)
{
	mySession = session;
}
void Game::UnitSpawn(uint16_t unitId, uint16_t playerId, Vector2 spawnPos)
{
	std::shared_ptr<Unit> unit = nullptr;
	if (_myId == playerId)
	{
		//_myPlayer->AddUnit(unitId);
		unit = std::make_shared<Unit>(spawnPos, playerId);
	}
	else
	{
		unit = std::make_shared<Unit>(spawnPos, Color::Red);
	}
	mainLevel->AddNewActor(unit.get());
	_units.try_emplace(unitId, unit);
}

void Game::UnitMove(uint16_t unitId, std::vector<Vector2> path)
{
	auto unitIter = _units.find(unitId);
	auto unit = unitIter->second;

	if (unit)
	{
		unit->MoveUnit(path);
	}
}

void Game::UnitAttacked(std::vector<uint16_t> unitsId)
{
	for (uint16_t unitId : unitsId)
	{
		auto unitIter = _units.find(unitId);
		auto unit = unitIter->second;

		if (unit)
		{
			unit->PlayDamageAnim();
		}
	}
}

void Game::UnitDespawn(uint16_t unitId)
{
	auto unitIter = _units.find(unitId);
	auto unit = unitIter->second;
	if (unit)
	{
		_units.erase(unitId);
		unit->Destroy();
	}
}

void Game::SendId(const std::string id)
{
	Packet loginPacket;
	loginPacket.write<uint16_t>(0);
	loginPacket.write<uint16_t>(PK_CS_ID);
	loginPacket.writeString(id);
	uint16_t packetSize = static_cast<uint16_t>(loginPacket.getBuffer().size());
	uint16_t networkSize = htons(packetSize);
	memcpy(loginPacket.getBuffer().data(), &networkSize, sizeof(uint16_t));

	mySession->DoSend(loginPacket.getBuffer().data(), packetSize);
}

Game& Game::Get()
{
	if (!instance)
	{
		std::cout << "instance is null\n";
		__debugbreak();
	}

	return *instance;
}
