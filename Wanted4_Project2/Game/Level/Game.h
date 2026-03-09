#pragma once
#include "Engine\Engine.h"
#include "Actor/Player.h"
#include "Actor/Npc.h"
#include "Actor/StunEffect.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>


using namespace Wanted;
class Player;
class Session;
class Unit;
//레벨 관리에 사용할 열거형

enum class State
{
	//None = -1, 이것도 많이 씀
	GamePlay = 0,
	Login = 1,
	Ready = 2,
	Length //상태 개수가 몇 개인지... 1씩 늘어나느 것만 가능
};

class Game : public Engine
{
public:
	Game();
	~Game();

	void GameStart(uint16_t width, uint16_t height, std::vector<uint8_t> tiles);

	void WaitingRoom(uint16_t playerId);
	
	//game seesion과 network 통신 session을 연결하는 함수
	void SetSession(const std::shared_ptr<Session> session);
	//void GameLevelStart(int width, int height, std::vector<uint8_t> tiles);

	void UnitSpawn(uint16_t unitId, uint16_t playerId, Vector2 spawnPos);
	void UnitMove(uint16_t unitId, std::vector<Vector2> path);
	void UnitAttacked(std::vector<uint16_t> unitsId);
	void UnitDespawn(uint16_t unitId);

	//아이디 패킷 전송?
	void SendId(const std::string id);

	//getter
	std::shared_ptr<Session>& getMySesseion() { return mySession; }

	std::unordered_map<uint16_t, std::shared_ptr<Unit>>& GetUnits() { return _units; }



	//싱글톤을 위한 겟 함수
	static Game& Get();

private:
	//게임에서 관리하는 레벨을 저장할 배열
	std::vector<Level*> levels;

	//현재 활성화된 레벨을 나타내는 상태 변수
	State state = State::Login;

	//싱글톤 구현을 위한 정적 변수
	static Game* instance;

	std::shared_ptr<Session> mySession;
	Player* _myPlayer = nullptr;

	uint16_t _myId;
	std::unordered_map<uint16_t, std::shared_ptr<Unit>> _units;
};

