#include "pch.h"
#include "Game.h"
#include "01.Game/Actor/GameSession.h"
#include "01.Game/Actor/Session.h"
#include "01.Game/Actor/Player.h"
#include "99.Header/PacketHandler.h"
#include "01.Game/Actor/Unit.h"

#include <cmath>

Game* Game::_instance = nullptr;

Game::Game()
	:_iocpHandle(nullptr), _overlappedPool(nullptr),
	_mapBoundary(_width / 2, _height / 2, _width / 2, _height / 2),
	_tree(std::make_unique<QuadTree>(Rect(_width / 2, _height / 2, _width / 2, _height / 2), 4))
{
	_instance = this;
}

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
	std::cout << "recv called - key: " << key
		<< " bytes: " << numbytes << "\n";

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
		if (packetSize == 0) break;
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
		PacketHandler::handlePacket(packet.data(), key, packetSize);
	}
	session->doRecv();
}

uint16 Game::getClientId()
{
	_clientId.fetch_add(1);
	return _clientId;
}

uint16 Game::getUnittId()
{
	_unitId.fetch_add(1);
	return _unitId;
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
	sprintf_s(path, 2048, "C:\\Wanted4_game\\Project\\Server\\Asset\\map.txt");

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


	// 줄바꿈/공백 제거
	mapStr.erase(std::remove(mapStr.begin(), mapStr.end(), '\r'), mapStr.end());
	mapStr.erase(std::remove(mapStr.begin(), mapStr.end(), '\n'), mapStr.end());
	mapStr.erase(std::remove(mapStr.begin(), mapStr.end(), ' '), mapStr.end());

	// 맵 크기 검증
	if (mapStr.size() < (size_t)(_width * _height))
	{
		std::cerr << "Map data size mismatch!\n";
		__debugbreak();
	}

	Packet mapPacket;
	mapPacket.write<uint16_t>(0);
	mapPacket.write<uint16_t>(PK_SC_GAME_START);
	mapPacket.write<uint16_t>(_width);
	mapPacket.write<uint16_t>(_height);

	int j = 0;
	int k = 0;

	// 가로 WIDTH, 세로 HEIGHT 크기로 초기화
	std::vector<uint8_t> packedData((_width * _height + 7) / 8, 0);
	_tiles.assign(_width * _height, 0);

	for (int i = 0; i < _width * _height; ++i)
	{
		int byteIdx = i / 8;
		int bitIdx = i % 8;

		// 타일이 '1'(벽)이면 해당 비트를 1로 셋팅
		if (mapStr[i] == '1')
		{
			_tiles[i] = 1;
			packedData[byteIdx] |= (1 << bitIdx);
		}
	}

	// 이후 packedData를 mapPacket에 write
	mapPacket.writeBuffer(packedData.data(), packedData.size());

	uint16_t totalSize = static_cast<uint16_t>(mapPacket.getBuffer().size());
	uint16_t networkSize = htons(totalSize);
	memcpy(&mapPacket.getBuffer()[0], &networkSize, sizeof(uint16_t));
	broadcast(mapPacket.getBuffer().data(), totalSize);
}

bool Game::isSpawnableUnit(uint16 playerId, Vector2 spawnPos)
{
	int tileX = (int)spawnPos.x;
	int tileY = (int)spawnPos.y;
	if (tileX < 0 || tileX > _width || tileY < 0 || tileY > _height) return false;

	//빈 공간만 허용
	if (_tiles[tileY * _width + tileX] != 0) return false;

	// 플레이어 1 → 왼쪽 절반
	if (playerId == 5)
	{
		if (spawnPos.x < _width / 2)
			return true;
		else
			return false;
	}
	// 플레이어 2 → 오른쪽 절반
	if (playerId == 6) {
		if (spawnPos.x >= _width / 2)
			return true;
		else
			return false;
	}
}

bool Game::isMovableUnit(uint16 playerId, Vector2 movePos)
{
	int tileX = (int)movePos.x;
	int tileY = (int)movePos.y;

	if (tileX < 0 || tileX > _width || tileY < 0 || tileY > _height) return false;

	return true;
}

void Game::spawnUnit(uint16 playerId, Vector2 spawnPos)
{
	if (!isSpawnableUnit(playerId, spawnPos))
		return;
	uint16 unitId = getUnittId();
	auto unit = std::make_shared<Unit>(spawnPos, unitId, playerId);
	addUnit(unit);
	auto player = players[playerId];
	if (!player->addUnit())
		return;

	unit->setMap(_tiles);

	{
		std::lock_guard<std::mutex>lock(_tileLock);
		_tiles[spawnPos.y * _width + spawnPos.x] = unitId;
	}

	Packet unitPacket;
	unitPacket.write<uint16>(0);
	unitPacket.write<uint16>(PK_SC_SPAWN_UNIT);
	unitPacket.write<uint16>(unitId);
	unitPacket.write<uint16>(playerId);
	unitPacket.write<Vector2>(spawnPos);
	uint16 packetSize = unitPacket.size();
	uint16 networkSize = htons(packetSize);
	memcpy(unitPacket.getBuffer().data(), &networkSize, sizeof(uint16_t));

	broadcast(unitPacket.getBuffer().data(), packetSize);

}

void Game::addUnit(std::shared_ptr<Unit> unit)
{
	{
		std::lock_guard<std::mutex>lock(_unitsLock);
		_units.try_emplace(unit->getId(), unit);

		_tree->insert({ unit->_pos.x, unit->_pos.y, unit.get() });
	}
}

void Game::removeUnit(uint16 unitId, uint16 playerId)
{
	Packet despawnUnit;
	despawnUnit.write<uint16>(0);
	despawnUnit.write<uint16>(PK_SC_DESPAWN_UNIT);
	despawnUnit.write<uint16>(unitId);
	uint16 packetSize = despawnUnit.size();
	uint16 networkSize = htons(packetSize);
	memcpy(despawnUnit.getBuffer().data(), &networkSize, sizeof(uint16_t));
	broadcast(despawnUnit.getBuffer().data(), packetSize);

	{
		std::lock_guard<std::mutex> lock(_unitsLock);
		auto unit = findUnit(unitId); 
		if (unit) {
			_tiles[unit->_pos.y * _width + unit->_pos.x] = 0;
		}
		_units.erase(unitId);
	}

	{
		std::lock_guard<std::mutex> lock(_treeLock);
		rebuild();
	}
	auto player = players[playerId];
	player->subUnit();
}

void Game::moveUnit(uint16 playerId, uint16 unitId, Vector2 movePos)
{
	if (!isMovableUnit(unitId, movePos))
	{
		return;
	}

	auto unit = findUnit(unitId);
	if (unit)
	{
		std::vector<Vector2> path = unit->moveUnit(movePos);

		if (path.empty())
			return;
		Vector2 goalPos = path.back();

		_tileLock.lock();
		_tiles[unit->_pos.y * _width + unit->_pos.x] = 0;
		_tiles[goalPos.y * _width + goalPos.x] = unitId;
		_tileLock.unlock();

		unit->_pos = goalPos;

		{
			std::lock_guard<std::mutex>lock(_treeLock);
			rebuild();
		}
		Packet movePacket;
		movePacket.write<uint16>(0);
		movePacket.write<uint16>(PK_SC_MOVE_UNIT);
		movePacket.write<uint16>(unitId);
		movePacket.write<uint16>(playerId);

		// 경로 개수 먼저 전송
		movePacket.write<uint16>(static_cast<uint16>(path.size()));

		// 경로 좌표 순서대로 전송
		for (const Vector2& pos : path)
		{
			movePacket.write<Vector2>(pos);
		}

		uint16 packetSize = movePacket.size();
		uint16 networkSize = htons(packetSize);
		memcpy(movePacket.getBuffer().data(), &networkSize, sizeof(uint16_t));
		broadcast(movePacket.getBuffer().data(), packetSize);
		std::cout << "MoveUnit " << unitId << "\n";
		_moveUnits.insert(unitId);
	}
	//
}

std::shared_ptr<Unit> Game::findUnit(uint16 unitId)
{
	auto it = _units.find(unitId);
	if (it == _units.end())return{};
	auto unit = it->second;
	if (!unit)return{};
	if (!unit->_isAlive) return {};
	return unit;
}

void Game::rebuild()
{
	_tree = std::make_unique<QuadTree>(_mapBoundary, _capacity);

	for (auto& [id, unit] : _units)
	{
		if (unit->_isAlive)
		{
			_tree->insert({ unit->_pos.x, unit->_pos.y, unit.get() });
		}
	}
}

void Game::attackUnit()
{
	std::set<std::pair<int, int>> combatPairs;
	//auto units = player->getUnits();

	for (auto& [id, unit] : _units)
	{
		std::vector<Unit*> nearby;
		_tree->queryCircle(unit->_pos.x, unit->_pos.y, unit->_range, nearby);

		for (Unit* other : nearby)
		{
			//같은 팀이면 넘김
			if (other->_ownerId == unit->_ownerId) continue;
			int dx = other->_pos.x - unit->_pos.x;
			int dy = other->_pos.y - unit->_pos.y;

			double dist = (dx * dx) + (dy * dy);
			if (dist <= unit->_range * unit->_range)
			{
				int a = min(unit->_id, other->_id);
				int b = max(unit->_id, other->_id);
				combatPairs.insert({ a, b });
			}
		}

	}
	std::vector<uint16> attakedUnits;
	for (auto& [idA, idB] : combatPairs)
	{
		auto unitA = findUnit(idA);
		auto unitB = findUnit(idB);
		
		if (!unitA || !unitB) return;

		double dx = unitA->_pos.x - unitB->_pos.x;
		double dy = unitA->_pos.y - unitB->_pos.y;
		double distSq = dx * dx + dy * dy; // sqrt 없이 제곱으로 비교 (더 빠름)

		// 각자 range 따로 체크
		if (distSq <= unitA->_range * unitA->_range)
		{
			unitB->takeDamage(unitA->_attack);
			attakedUnits.push_back(unitB->getId());
		}

		if (distSq <= unitB->_range * unitB->_range)
		{
			unitA->takeDamage(unitB->_attack);
			attakedUnits.push_back(unitA->getId());

		}
		attackedUnit(attakedUnits);
	}

	Timer event{ 0, std::chrono::system_clock::now() + 3s, TimerEvent::EV_UNIT_ATTACK, 0 };

	//타이버 이벤트를 넣기 위한 뮤텍스 처리
	std::lock_guard<std::mutex> lock(timer_mutex);
	timer_queue.push(event);
	timer_cv.notify_one();
}

void Game::attackedUnit(std::vector<uint16> attackedUnits)
{
	Packet attackedUnit;
	attackedUnit.write<uint16>(0);
	attackedUnit.write<uint16>(PK_SC_ATTACK_UNIT);
	attackedUnit.write<uint16>(attackedUnits.size());
	for (uint16 id : attackedUnits)
	{
		attackedUnit.write<uint16>(id);
	}
	uint16 packetSize = attackedUnit.size();
	uint16 networkSize = htons(packetSize);
	memcpy(attackedUnit.getBuffer().data(), &networkSize, sizeof(uint16_t));
	broadcast(attackedUnit.getBuffer().data(), packetSize);
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
	waitingRoom.write<uint16_t>(id);
	uint16_t totalSize = static_cast<uint16_t>(waitingRoom.getBuffer().size());
	uint16_t networkSize = htons(totalSize);
	memcpy(&waitingRoom.getBuffer()[0], &networkSize, sizeof(uint16_t));

	std::shared_ptr<Session> session = sessions[id];
	session->doSend(waitingRoom.getBuffer().data(), totalSize);

	Timer event{ id, std::chrono::system_clock::now() + 2s, TimerEvent::EV_GAME_START, 0 };

	//타이버 이벤트를 넣기 위한 뮤텍스 처리
	std::lock_guard<std::mutex> lock(timer_mutex);
	timer_queue.push(event);
	timer_cv.notify_one();
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

