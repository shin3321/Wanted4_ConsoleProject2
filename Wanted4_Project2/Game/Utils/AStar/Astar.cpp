#include "Astar.h"

#include <cmath>
#include <algorithm>

AStar::AStar()
	:_startNode(nullptr), _goalNode(nullptr)
{
}

AStar::~AStar()
{
	clearLists();
	SafeDelete(_goalNode);
}

std::vector<Vector2> AStar::FindPath(Vector2 startPos, Vector2 goalPos, uint16_t _id)
{
	clearLists();

	Node* startGridNode = getNode(startPos);
	Node* goalGridNode = getNode(goalPos);


	if (!startGridNode || !goalGridNode || _mapData->empty())
		return {};

	_startNode = new Node(startPos, nullptr);
	_goalNode = new Node(goalPos, nullptr);

	_startNode->_gCost = 0;
	_startNode->_hCost = calculateHeuristic(_startNode, _goalNode);
	_startNode->_fCost = _startNode->_hCost;
	Node* closestNode = _startNode;
	// 대각선 이동 비용 상수.
	const float diagonalCost = 1.5;

	//시작 노드를 열린 리스트에 추가
	_openList.emplace_back(_startNode);

	// 상하좌우 및 대각선 이동 방향과 비용 => 순서 바꾸지 말 것
	//dx * 2 + dy = 대각선임
	std::vector<Direction> directions =
	{
		// 하상우좌 이동.
		{ 0, 1, 1.0f }, { 0, -1, 1.0f }, { 1, 0, 1.0f }, { -1, 0, 1.0f },

		// 대각선 이동 (우하, 우상, 좌하, 좌상)
		{ 1, 1, diagonalCost }, { 1, -1, diagonalCost }, { -1, 1, diagonalCost }, { -1, -1, diagonalCost }
	};

	while (!_openList.empty())
	{
		Node* lowestNode = _openList[0];
		for (Node* node : _openList)
		{
			if (node->_fCost < lowestNode->_fCost)
			{
				lowestNode = node;
			}
		}

		//fcost가 가장 낮은 노드를 현재 노드로 설정
		Node* currentNode = lowestNode;
		//ClosedNode = currentNode;


		//현재 노드가 목적지면 경로 보내기
		if (isDestination(currentNode))
		{

			//Game::get().setUnitPosTiles(currentNode->_pos, _startNode->_pos, _id);
			std::vector<Vector2> path = constructPath(currentNode);
			clearLists();
			SafeDelete(_goalNode); // goalNode는 리스트 밖에 있으니 별도 삭제
			return path;
		}

		//방문 처리를 위해 현재 노드를 열린 리스트에서 제거
		for (int i = 0; i < static_cast<int>(_openList.size()); ++i)
		{
			if (_openList[i] == currentNode)
			{
				_openList.erase(_openList.begin() + i);
				break;
			}
		}

		//방문 처리를 위해 현재 노드를 닫힌 리스트에 추가
		_closedList.emplace_back(currentNode);

		if (currentNode->_hCost < closestNode->_hCost)
			closestNode = currentNode;


		//이웃노드 방문(탐색)
		for (const Direction& direction : directions)
		{
			int newX = currentNode->_pos.x + direction.x;
			int newY = currentNode->_pos.y + direction.y;

			//범위 밖이면 무시
			if (!isInRange(newX, newY))
			{
				continue;
			}

			//이동할 위치가 장애물인 경우에는 무시
			if ((*_mapData)[newY * MAP_WIDTH + newX] != 0)
			{
				continue;
			}

			// 대각선이면 두 축 방향 체크
			if (direction.x != 0 && direction.y != 0)
			{
				if ((*_mapData)[currentNode->_pos.y * MAP_WIDTH + newX] == 1 ||
					(*_mapData)[newY * MAP_WIDTH + currentNode->_pos.x] == 1)
					continue;
			}

			//현재 노드르 기준으로 새 gCost 계산
			float newGCost = currentNode->_gCost + direction.cost;

			//이미 더 좋은 비용으로 방문한 노드인 경우 무시
			if (hasVisited(newX, newY, newGCost))
			{
				continue;
			}

			//방문을 위한 이웃 노드 생성
			Node* neighborNode = new Node(Vector2(newX, newY), currentNode);

			//방문할 노드의 gCost 계산
			neighborNode->_gCost = newGCost;
			//방문할 노드의 hCost 계산
			neighborNode->_hCost = calculateHeuristic(neighborNode, _goalNode);

			//방문할 노드의 fCost
			neighborNode->_fCost = neighborNode->_gCost + neighborNode->_hCost;

			//이웃 노드가 열린 리스트에 있는지 확인
			Node* openListNode = nullptr;
			for (Node* node : _openList)
			{
				if (*node == *neighborNode)
				{
					openListNode = node;
					break;
				}
			}

			if (openListNode != nullptr)
			{
				//이웃 노드가 열린 리스트에 있으면 더 좋은 경로인 경우 비용만 갱신
				if (neighborNode->_gCost < openListNode->_gCost || neighborNode->_fCost < openListNode->_fCost)
				{
					// 부모 노드 갱신.
					openListNode->parentNode = currentNode;
					// 비용 정보 갱신.
					openListNode->_gCost = neighborNode->_gCost;
					// 비용 정보 갱신.
					openListNode->_hCost = neighborNode->_hCost;
					// 비용 정보 갱신.
					openListNode->_fCost = neighborNode->_fCost;
				}
				SafeDelete(neighborNode);
				continue;
			}

			// 새로 방문 가능한 노드를 열린 리스트에 추가.
			if ((*_mapData)[newY * MAP_WIDTH + newX] == 0)
			{
				// 방문 시각화 값 설정.
			//	(*_mapData)[newY * MAP_WIDTH + newX] = 5;
			}
			_openList.emplace_back(neighborNode);
		}
	}

	if (closestNode == _startNode)
	{
		clearLists();
		SafeDelete(_goalNode);
		return {}; // 시작점 외에 갈 곳이 없으면 취소
	}

	std::vector<Vector2> path = constructPath(closestNode);

	clearLists();
	SafeDelete(_goalNode);
	return path;
}

void AStar::setMapData(std::vector<uint8_t>& mamData)
{
	_mapData = &mamData;
	initGrid();
}

Node* AStar::getNode(Vector2 pos)
{
	int x = static_cast<int>(pos.x);
	int y = static_cast<int>(pos.y);
	return &_nodeGrid[y * MAP_WIDTH + x];
}

void AStar::clearLists()
{
	for (Node* node : _openList)
		SafeDelete(node);
	_openList.clear();

	for (Node* node : _closedList)
		SafeDelete(node);
	_closedList.clear();
}

void AStar::initGrid()
{
	_nodeGrid.resize(MAP_WIDTH * MAP_HEIGHT);
	for (int y = 0; y < MAP_HEIGHT; y++)
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int idx = y * MAP_WIDTH + x;
			_nodeGrid[idx]._pos = { x, y };
			_nodeGrid[idx]._isWall = ((*_mapData)[idx] != 0);
		}
}

std::vector<Vector2> AStar::constructPath(Node* goalNode)
{
	std::vector<Vector2> path;
	Node* currentNode = goalNode;
	while (currentNode != nullptr)
	{
		path.emplace_back(currentNode->_pos);
		currentNode = currentNode->parentNode;
	}
	std::reverse(path.begin(), path.end());

	return path;
}

int AStar::calculateHeuristic(Node* currentNode, Node* goalNode)
{
	// 현재 노드의 위치에서 목표 위치까지의 거리를 휴리스틱 값으로 사용.
	Vector2 diff = *currentNode - *goalNode;
	return static_cast<float>(std::sqrt(std::pow(diff.x, 2) + std::pow(diff.y, 2)));
}

bool AStar::isInRange(int x, int y)
{
	// 빈 그리드면 탐색할 수 없으므로 false 반환.
	if ((*_mapData).empty())
	{
		return false;
	}

	// x, y 범위가 벗어나면 false 반환.
	if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
	{
		return false;
	}

	// 벗어나지 않았으면 true 반환.
	return true;
}

bool AStar::hasVisited(int x, int y, float gCost)
{
	// 열린 리스트에 이미 같은 위치가 있고 비용이 더 좋거나 같으면 방문했다고 판단.
	for (Node* const node : _openList)
	{
		if (node->_pos.x == x && node->_pos.y == y && gCost >= node->_gCost)
		{
			return true;
		}
	}

	// 닫힌 리스트에 이미 같은 위치가 있고 비용이 더 좋거나 같으면 방문했다고 판단.
	for (Node* const node : _closedList)
	{
		if (node->_pos.x == x && node->_pos.y == y && gCost >= node->_gCost)
		{
			return true;
		}
	}

	// 리스트에 없거나 더 좋은 비용이면 방문하지 않은 것으로 판단.
	return false;
}


bool AStar::isDestination(const Node* node)
{
	return(*node == *_goalNode && (*_mapData)[node->_pos.y * MAP_WIDTH + node->_pos.x] == 0);
}

bool AStar::isMovable(int x, int y)
{
	return false;
}