#pragma once

#include <iostream>
#include <string>
#include <winsock2.h>
#include <mswsock.h>
#include <Windows.h>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string.h> 

#include "99.Header/Protocol.h"
#include "99.Header/Vector2.h"
#include "99.Header/Types.h"
#include "02.Network/OverlappedEx.h"
#include "02.Network/RingBuffer.h"
#include "02.Network/TimerEvent.h"
#include "Packet.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

