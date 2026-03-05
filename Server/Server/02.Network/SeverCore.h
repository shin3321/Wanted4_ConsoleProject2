#pragma once

class SeverCore
{

public:
	SeverCore();
	~SeverCore();
	//iocp 객체 생성
	void init();
	void registerHandle(HANDLE handle);

	//getqueuecompletionstatus
	void runWorkThread();

	//core 개수
	int getCore();
	
	//getter
	HANDLE getHandle() { return _iocpHandle; }

private:


private:
	//서버 연결 소켓
	SOCKET _listenSocket;
	SOCKET _clientSocket;

	//accpet용 overlapped 구조체
	OverlappedEx _acceptOverlapped;
	OverlappedExPool _overlappedPool;

	HANDLE _iocpHandle;
};

