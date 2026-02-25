#pragma once
#include <Windows.h>
/*----------------
	 IocpCore
----------------*/

class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, __int32 numOfBytes = 0) abstract;
};

//iocp handle을 관리하는 클래스, 유일하게 handle을 들고 있음
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	//getter
	HANDLE getHandle() { return _iocpHandle; }

	//createCompletionPort
	bool registerHandle(class IocpObject* iocpObj);

	//GetQueueCompletionStatus
	bool dispatchHandle(unsigned __int32 timeOutMs = INFINITE);

	//싱글톤 겟 함수
	static IocpCore get();
private:
	HANDLE _iocpHandle = INVALID_HANDLE_VALUE;
};

//핸들 전역 변수
IocpCore* giocpHandle;
