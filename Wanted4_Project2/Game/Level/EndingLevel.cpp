#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "EndingLevel.h"
#include "Render/Renderer.h"
#include "Math/Vector2.h"
#include "Engine/Engine.h"
#include "Game.h"

EndingLevel::EndingLevel(int victory):_vic(victory)
{
	_timer.SetTargetTime(4.0f);
}

void EndingLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	_timer.Tick(deltaTime);

	if (_timer.IsTimeOut())
	{

	}

}

void EndingLevel::Draw()
{
	super::Draw();
	//승리 시
	if(_vic == 0 )
	{
		Renderer::Get().Submit("Win", Vector2(Engine::Get().GetWidth() / 2, Engine::Get().GetHeight() / 2), Color::Blue);
	}
	else if(_vic ==1)
		Renderer::Get().Submit("Lose", Vector2(Engine::Get().GetWidth() / 2, Engine::Get().GetHeight() / 2), Color::Red);
	else
		Renderer::Get().Submit("Draw", Vector2(Engine::Get().GetWidth() / 2, Engine::Get().GetHeight() / 2), Color::Green);

}
