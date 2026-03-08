
#define WIN32_LEAN_AND_MEAN
#include "LoginLevel.h"
#include "Render/Renderer.h"
#include "Core/Input.h"
#include "Game.h"

#include <conio.h>
#include <iostream>

LoginLevel::LoginLevel()
{
	Renderer::Get().Submit("StarCraft", Vector2(Engine::Get().GetWidth() / 2, Engine::Get().GetHeight() / 2 - 10));
	Renderer::Get().Submit("Id", Vector2(Engine::Get().GetWidth() / 2 - 5, Engine::Get().GetHeight() / 2));

}

LoginLevel::~LoginLevel()
{
}

void LoginLevel::Tick(float deltaTime)
{
	UpdateId();
	super::Tick(deltaTime);

	// 종료 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 게임 종료.
		Engine::Get().QuitEngine();
	}
}

void LoginLevel::Draw()
{
	super::Draw();

	Renderer::Get().Submit("StarCraft", Vector2(Engine::Get().GetWidth() / 2, Engine::Get().GetHeight() / 2 - 10));
	Renderer::Get().Submit("Id", Vector2(Engine::Get().GetWidth() / 2 - 5, Engine::Get().GetHeight() / 2));

	Renderer::Get().Submit(
		(inputId).c_str(),
		Vector2(Engine::Get().GetWidth() / 2 - 2, Engine::Get().GetHeight() / 2));

	Renderer::Get().Submit(
		(inputId).c_str(),
		Vector2(Engine::Get().GetWidth() / 2 - 2, Engine::Get().GetHeight() / 2));
}

void LoginLevel::UpdateId()
{
	// 키 입력이 있을 때만 로직을 수행

	char ch = _getch();
	//13 == enter
	if (ch == 13)
	{
		Game::Get().SendId(inputId);
	}
	//8 == backspace
	else if (ch == 8)
	{
		if (!inputId.empty())
		{
			inputId.pop_back();
		}
	}
	else if (ch == 27)
	{
		//Engine::Get().Shutdown();
	}
	else
	{
		inputId.push_back(ch);

	}
}
