#pragma once
#include "01.Game/Actor/Unit.h"

struct Point {
    int x, y;
    Unit* unit;  // 실제 유닛 포인터
    Point(int x, int y, Unit* u) : x(x), y(y), unit(u) {}
};