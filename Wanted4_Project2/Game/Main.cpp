#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Network/Network.h"
#include "Engine/Engine.h"
#include "Level/ProcessPacket.h"
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
	ProcessPacket process;
	std::thread networkThread([&net]()
		{
			net.Init();
		});

	std::thread processTread([&process]()
		{
			process.ProcessPackets();
		});

	game.Run();

	net.ShutDown();
	process.ShutDown();
	if (networkThread.joinable()) {
		networkThread.join();
	}
	if (processTread.joinable()) {
		processTread.join();
	}
}
