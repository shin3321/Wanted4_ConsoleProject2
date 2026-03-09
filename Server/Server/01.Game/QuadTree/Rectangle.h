#pragma once
#include "01.Game/QuadTree/Bound.h"

struct Rect
{
	int _x, _y, _w, _h;
	Rect(int x, int y, int w, int h)
		:_x(x), _y(y), _w(w), _h(h) {
	}

	bool contains(const Point& p) const
	{
		return (_x - _w <= p.x && p.x <= _x + _w &&
			_y - _h <= p.y && p.y <= _y + _h);
	}

	bool intersects(const Rect& o) const
	{
		return !(o._x - o._w > _x + _w || o._x + o._w < _x - _w ||
			o._y - o._h > _y + _h || o._y + o._h < _y - _h);
	}
};

