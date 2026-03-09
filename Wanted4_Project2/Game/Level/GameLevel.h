#pragma once
#include "Level\ManageLevel.h"
#include "Math/Color.h"
#include "Utils/Timer.h"

#include <string>

using namespace Wanted;

class GameLevel : public ManageLevel
{
	RTTI_DECLARATIONS(GameLevel, ManageLevel)

public:
	GameLevel();
	GameLevel(int width, int height, std::vector<uint8_t> tiles);
	~GameLevel();

	// 게임 플레이 이벤트.
	virtual void BeginPlay();
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

public:
	//void SetPlayer(uint16_t playerId);
	//std::shared_ptr<Player> GetPlayer() { return _myPlayer; }

private:
	int _width;
	int _height;
	std::vector<uint8_t> _tiles;

	////스코어 관련
	//int score1 = 0;
	//int score2 = 0;

	//char score1String[128] = {};
	//char score2String[128] = {};

	//시간 관련
	Timer gameTimer;
};

