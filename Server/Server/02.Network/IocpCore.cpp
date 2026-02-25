#include "pch.h"
#include "IocpCore.h"

IocpCore::IocpCore()
{
	_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
}

IocpCore::~IocpCore()
{
}

bool IocpCore::registerHandle(IocpObject* iocpObj)
{
	return ::CreateIoCompletionPort(iocpObj->GetHandle(), _iocpHandle, reinterpret_cast<ULONG_PTR>(iocpObj),0 );
}

bool IocpCore::dispatchHandle(unsigned __int32 timeOutMs)
{
	return false;
}
