#include "pch.h"
#include "Game.h"
#include "01.Game/Actor/GameSession.h"
#include "01.Game/Actor/Session.h"
#include "99.Header/PacketHandler.h"

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
	auto newSession = std::make_shared<Session>(*_overlappedPool);
	newSession->_socket = socket;
	newSession->_id = newId;
	sessions.try_emplace(newId, newSession);

	//세션에 넣은 소켓 io 등록
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(newSession->_socket), _iocpHandle, newSession->_id, 0);

	//recv 등록
	newSession->doRecv();
	std::cout << "new Session " << newSession->_id << "\n";
}

void Game::recv(uint16_t key, uint16_t numbytes)
{
	//받은 세션 아이디 확인
	std::shared_ptr<Session> session = sessions[key];
	if (!session)
	{
		std::cout << "Invalid Session In Recv. Id: " << key << "\n";
		return;
	}

	if (!session->_recvBuf.write(session->_recvOver._wsaBuf.buf, numbytes))
	{
		//Todo: Recv Error
	}

	while (true)
	{
		//recv 버퍼에 패킷 사이즈가 없으면 실패
		uint16_t packetSize = 0;
		if (!session->_recvBuf.peek(reinterpret_cast<char*>(&packetSize), sizeof(uint16_t)))
			break;
		packetSize = ntohs(packetSize);

		//받은 리시브 버퍼의 크기가 패킷 사이즈보다 작으면 실패
		if (session->_recvBuf.size() < packetSize)
			break;

		//받은 패킷을 받을 벡터 만들기
		std::vector<char> packet(packetSize);

		if (!session->_recvBuf.read(packet.data(), packetSize))
		{
			std::cerr << " Ring Buffer Read Failed";
			break;
		}

		//프로세스 패킷
		PacketHandler::handlePacket(packet.data(), key);
	}
}

int Game::getClientId()
{
	_clientId.fetch_add(1);
	return _clientId;
}

void Game::MapLoad()
{

}

void Game::closeSocket(int sessioneId)
{
	std::shared_ptr<Session> session = sessions[sessioneId];
	if (!session)
	{
		std::cout << "Invalid Session In Recv. Id: " << sessioneId << "\n";
		return;
	}
	closesocket(session->_socket);
}

void Game::loadMap()
{
	char path[2048] = {};
	sprintf_s(path, 2048, "C:\\Wanted4_game\\CppBasic\\ConsoleEngine\\BugGameServer\\Assets\\map.txt");

	//파일 열기
	FILE* file = nullptr;
	fopen_s(&file, path, "rt");

	//예외 처리
	if (!file)
	{
		//표준 오류 콘솔 활용
		std::cerr << "Failed to open  map file.\n";
		//.디버그 모드엣 ㅓ중단점으로 중단해 줌
		__debugbreak();
	}

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	//파일 데이터를 읽어올 버퍼 생성
	char* data = new char[fileSize + 1];
	size_t readSize = fread(data, sizeof(char), fileSize, file);
	data[readSize] = '\0';
	fclose(file);

	//맵 데이터 파싱 및 서버 저장
	std::string mapStr(data);
	delete[] data;

	std::vector<std::string> lines;
	Packet mapPacket;
	mapPacket.write<uint16_t>(0);
	mapPacket.write<uint16_t>(PK_SC_GAME_START);
	mapPacket.write<uint16_t>(_width);
	mapPacket.write<uint16_t>(_height);
							  
	int j = 0;
	int k = 0;

	// 가로 WIDTH, 세로 HEIGHT 크기로 초기화
	_tiles.assign(_height, std::vector<uint16_t>(_width, 0));

	for (int i = 0; i < mapStr.length(); ++i)
	{
		char c = mapStr[i];
		if (c == '\n' || c == '\r')
		{
			++j;
			k = 0;
			continue;
		}

		uint8_t tileType = 0;
		if (c == '1') tileType = 1;
		else  tileType = 0;

		_tiles[j][k] = tileType; k++;
		mapPacket.write<uint8_t>(tileType);
	}

	uint16_t totalSize = static_cast<uint16_t>(mapPacket.getBuffer().size());
	uint16_t networkSize = htons(totalSize);
	memcpy(&mapPacket.getBuffer()[0], &networkSize, sizeof(uint16_t));
	broadcast(mapPacket.getBuffer().data(), totalSize);
}

void Game::waitingRoom(uint16_t id)
{
	if (sessions.size() == 2)
	{
		//Todo 2명이면 3초 뒤에 게임 시작 모두에게 보냄
	//	TimerEvent event{i}
	}

	Packet waitingRoom;
	waitingRoom.write<uint16_t>(0);
	waitingRoom.write<uint16_t>(PK_SC_WAITING);
	uint16_t totalSize = static_cast<uint16_t>(waitingRoom.getBuffer().size());
	uint16_t networkSize = htons(totalSize);
	memcpy(&waitingRoom.getBuffer()[0], &networkSize, sizeof(uint16_t));

	std::shared_ptr<Session> session = sessions[id];
	session->doSend(waitingRoom.getBuffer().data(), totalSize);
	Timer event{ id, std::chrono::system_clock::now() + 2s, TimerEvent::EV_GAME_START, 0};
}

void Game::broadcast(const char* data, uint16 packetSize)
{
	for (const auto session : sessions)
	{
		session.second->doSend(data, packetSize);
	}
	
}

void Game::setPlayer(std::shared_ptr<Player> player, uint16_t playerId)
{
	//Todo players
	players.try_emplace(playerId, player);
}

std::shared_ptr<Session> Game::getSession(uint16_t id)
{
	std::shared_ptr<Session> session = sessions[id];
	return session;
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
