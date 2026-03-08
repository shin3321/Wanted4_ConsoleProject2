#include "Wall.h"

Wall::Wall(const Vector2& position) : super("#", position, Color::White)
{
	//그리기 우선 순위 결정
	sortingOrder = 1;
}
