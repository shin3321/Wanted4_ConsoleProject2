#pragma once
#include <vector> 
class INetwork
{
public:
	virtual void DoSend(void* packet, uint16_t packetSize)=0;
	virtual void DoRecv()=0;
};

