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

	Renderer::Get().Submit("Game", Vector2::Zero);
//	Renderer::Get().SubmitFile("mify.txt", Vector2::Zero);

}
