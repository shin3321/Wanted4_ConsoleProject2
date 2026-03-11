#pragma once
#include "01.Game/QuadTree/Rectangle.h"

class QuadTree
{
public:
	QuadTree(const Rect& b, int cap = 4) : _boundary(b), _capacity(cap) {}

	~QuadTree()
	{
	};

	//원형 범위로 탐색 -> attack range가 원형임
	void queryCircle(int cx, int cy, int radius, std::vector<Unit*>& found) const;
	bool insert(const Point& p);

private:
	//노드가 4개가 다 찼을 때 다시 쪼개는 함수
	void subdivide();

	bool insertChild(const Point& p);

private:
	Rect _boundary;
	int _capacity;
	std::vector<Point> _points;
	bool _isDivided = false;
	std::unique_ptr<QuadTree> nw, ne, sw, se;
};

