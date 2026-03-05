#include "pch.h"
#include "Game.h"
#include "01.Game/Actor/GameSession.h"
#include "01.Game/Actor/Session.h"

Game* Game::_instance = nullptr;

Game::~Game()
{
}

void Game::init(HANDLE iocpHandle, OverlappedExPool& overlappedPool)
{
	_iocpHandle = iocpHandle;
	_overlappedPool = &overlappedPool;
}

void Game::accept(SOCKET socket)
{
	std::cout << "accept\n";
	int newId = getClientId();
	auto newSession = std::make_shared<Session>();
	newSession->_socket = socket;
	newSession->_id = newId;
	sessions.try_emplace(newId, newSession);

	//세션에 넣은 소켓 io 등록
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(newSession->_socket), _iocpHandle, newSession->_id, 0);

	newSession->doRecv();
	std::cout << "new Session " << newSession->_id;
}

void Game::recv(uint16_t key)
{
	std::shared_ptr<Session> seesion = sessions[key];

}

int Game::getClientId()
{
	_clientId.fetch_add(1);
	return _clientId;
}

void Game::MapLoad()
{

}

Game& Game::get()
{
	if (!_instance)
	{
		std::cout << "instance is null";
		__debugbreak();
	}
	return *_instance;
}

Game::Game()
	:_iocpHandle(nullptr), _overlappedPool(nullptr)
{
	_instance = this;
}
