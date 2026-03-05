#pragma once

class Session;
class Game
{
public:
	~Game();
	Game();
	//Game 초기화

	void init(HANDLE iocpHandle, OverlappedExPool& overlappedPool);
	void runWorkerThread(SOCKET socket);
	void accept(SOCKET socket);
	//서버 recv
	void recv(uint16_t key);

	//클라이언트 아이디 생성
	int getClientId();

	//맵 생성
	void MapLoad();

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
	std::map<int, std::shared_ptr<Session>> sessions;
};

