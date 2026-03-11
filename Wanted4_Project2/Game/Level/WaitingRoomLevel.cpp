#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "WaitingRoomLevel.h"
#include "Render/Renderer.h"

#include "Core/Input.h"
#include "Game.h"

WaitingRoomLevel::WaitingRoomLevel()
{

}

WaitingRoomLevel::~WaitingRoomLevel()
{
}

void WaitingRoomLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void WaitingRoomLevel::Draw()
{
	super::Draw();

	Renderer::Get().Submit("Waiting...", Vector2(Engine::Get().GetWidth() /2, Engine::Get().GetHeight() / 2), Color::LightBlue);
//	Renderer::Get().SubmitFile("mify.txt", Vector2::Zero);

}
