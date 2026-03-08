#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Network/Network.h"
#include "Engine/Engine.h"
#include "Level/WaitingRoomLevel.h"
#include "Level/Game.h"

#include <thread>
using namespace Wanted;

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	Game game;
	Network net;
	std::thread networkThread([&net]()
		{
			net.Init();
		});

	game.Run();

	net.ShutDown();
	if (networkThread.joinable()) {
		networkThread.join();
	}
}
