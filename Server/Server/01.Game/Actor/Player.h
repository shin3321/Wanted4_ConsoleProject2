#pragma once
//===============================
//		    Player
// 캐릭터 스탯, 현재 속한 방 정보 등 게임 로직을 수행
// 유저 개인의 고유 정보
//===============================

class GameSession;
class Unit;
class Player
{
public:
	Player(std::string nickname);

public:
	void addUnit(uint16 unitId, std::shared_ptr<Unit> unit);
	std::map<uint16, std::shared_ptr<Unit>> getUnits() { return _units; }
private:
	//std::shared_ptr<GameSession> _session;

	std::string _nickname;

	std::map<uint16, std::shared_ptr<Unit>> _units;
};

