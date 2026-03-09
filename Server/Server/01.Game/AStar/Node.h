#pragma once

#include <iostream>
#include <99.Header/Vector2.h>

class AStar;
class Node
{
	friend class AStar;
public:
	Node()
		:_pos(0, 0), parentNode(nullptr), _gCost(0), _hCost(0), _fCost(0)
	{
	}

	Node(Vector2 pos, Node* parentNode = nullptr)
		:_pos(pos), _gCost(0), _hCost(0), _fCost(0), parentNode(parentNode)
	{
	}

	~Node()
	{
	}

	Vector2 operator-(const Node& otherNode)
	{
		return Vector2(_pos.x - otherNode._pos.x, _pos.y - otherNode._pos.y);
	}

	bool operator==(const Node& otherNode) const
	{
		return _pos.x == otherNode._pos.x && _pos.y == otherNode._pos.y;
	}

private:
	Vector2 _pos;
	float  _gCost;
	float  _hCost;
	float  _fCost;

	bool _isWall = 0;
	Node* parentNode;

};
