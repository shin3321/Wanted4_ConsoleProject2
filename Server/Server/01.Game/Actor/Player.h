#pragma once
//===============================
//		    Player
// 캐릭터 스탯, 현재 속한 방 정보 등 게임 로직을 수행
// 유저 개인의 고유 정보
//===============================

class GameSession;
class Player
{
public:
	Player(std::string nickname);

public:
	void setNickname(std::string nickname);

private:
	//std::shared_ptr<GameSession> _session;

	std::string _nickname;
};

