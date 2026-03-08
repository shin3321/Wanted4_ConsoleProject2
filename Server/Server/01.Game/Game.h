#pragma once
class Player;
class Session;
//class Unit;

class Game
{
public:
	~Game();
	Game();
	//Game 초기화

	void init(HANDLE iocpHandle, OverlappedExPool& overlappedPool);
	//void runWorkerThread(SOCKET socket);

	//accept 함수 - 플레이어와 연결 후 sessions에 넣음 
	void accept(SOCKET socket);

	//서버 recv
	void recv(uint16_t key, uint16_t numbytes);

	//클라이언트 아이디 생성
	uint16 getClientId();
	uint16 getUnittId();

	//맵 생성
	void MapLoad();

	//closeSocket 함수 - 플레이어의 소켓을 닫는 함수
	void closeSocket(int sessioneId);
	void broadcast(const char* data, uint16 packetSize);

	//맵 전송 함수
	void loadMap();
	void waitingRoom(uint16 id);

	//유닛을 생성할 수 있는 곳인지 판단하는 함수
	bool isSpawnableUnit(uint16 playerId, Vector2 spawnPos);
	bool isMovableUnit(uint16 playerId, Vector2 movePos);

	//유닛을 생성하는 함수
	void spawnUnit(uint16 playerId, Vector2 spawnPos);
	void moveUnit(uint16 playerId, uint16 unitId, Vector2 movePos);

	//setter,getter
	//아이디에 맞는 세션을 반환	
	void setPlayer(std::shared_ptr<Player> player, uint16_t playerId);
	std::shared_ptr<Session> getSession(uint16_t id);

	//싱글톤 함수
	static Game& get();

private:
	//싱글톤 정적 변수
	static Game* _instance;

	//클라이언트 아이디
	std::atomic<uint16> _clientId = 5;

	//unitId
	std::atomic<uint16> _unitId = 10;

	HANDLE _iocpHandle;
	OverlappedExPool* _overlappedPool;

private:
	//맵 너비, 높이
	uint16_t _width = 256;
	uint16_t _height = 95;
	std::vector<uint8_t> _tiles;

private:
	std::map<uint16, std::shared_ptr<Session>> sessions;
	std::map<uint16, std::shared_ptr<Player>> players;

	//std::unordered_map<uint16, std::shared_ptr<Unit>> _player2Units;
};

