#include "GameLevel.h"
#include "Math/Vector2.h"
#include "Actor/Wall.h"
#include "Render/Renderer.h"
#include "Math/Color.h"
#include "Util/Util.h"

GameLevel::GameLevel()
{
}

GameLevel::GameLevel(int width, int height, std::vector<uint8_t> tiles)
{
	_width = width;
	_height = height;
	Vector2 position{ 0,0 };
	for (int y = 0; y < _height; ++y)
	{
		position.x = 0;
		for (int x = 0; x < _width; ++x)
		{
			int i = (y * width) + x;
			uint8_t type = tiles[i];
			if (type == 1) AddNewActor(new Wall(position));
			++position.x;
		}
		++position.y;
	}
	gameTimer.SetTargetTime(120.0f);
}

GameLevel::~GameLevel()
{
	
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
	Util::TurnOffCursor();
	Util::DisableConsoleSelection();
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void GameLevel::Draw()
{
	super::Draw();
	//Renderer::Get().Submit("Game", Vector2::Zero);
}
