#pragma once
class Player;
class Session;
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
	int getClientId();

	//맵 생성
	void MapLoad();

	//closeSocket 함수 - 플레이어의 소켓을 닫는 함수
	void closeSocket(int sessioneId);

	//맵 전송 함수
	void loadMap();
	void waitingRoom(uint16 id);

	//게임 시작 함수
	void sendGameStart();

	void broadcast(const char* data, uint16 packetSize);

	//--------setter-----------
	void setPlayer(std::shared_ptr<Player> player, uint16_t playerId);


	//--------getter-----------
	// 
	//아이디에 맞는 세션을 반환
	std::shared_ptr<Session> getSession(uint16_t id);



	//싱글톤 함수
	static Game& get();

private:
	//싱글톤 정적 변수
	static Game* _instance;

	//클라이언트 아이디
	std::atomic<int> _clientId = 0;

	HANDLE _iocpHandle;
	OverlappedExPool* _overlappedPool;

private:
	//맵 너비, 높이
	uint16_t _width = 256;
	uint16_t _height = 128;
	std::vector<std::vector<uint16_t>> _tiles;

private:
	std::map<int, std::shared_ptr<Session>> sessions;
	std::map<int, std::shared_ptr<Player>> players;
};

