#pragma once

template<typename T>
void SafeDelete(T*& pointer)
{
	if (pointer != nullptr)
	{
		delete pointer;
		pointer = nullptr;
	}
}

const int MAP_HEIGHT = 95;
const int MAP_WIDTH = 256;

class Node;
class AStar
{
private:
	struct Direction
	{
		int x, y;

		//비용
		float cost;
	};


public:
	AStar();
	~AStar();

	//경로 검색 함수
	std::vector<Node*> FindPath(Node* starNode, Node* goalNode);

	//맵 전달
	void setMapData(std::vector<uint8>& mamData);

private:
	//탐색을 마친 후 최적 경로를 반환하는 함수
	std::vector<Node*> constructPath(Node* goalNode);

	//hCost 계산 함수
	int calculateHeuristic(Node* currentNode, Node* goalNode);

	//탐색하려는 위치가 그리드 범위 안에 있는지 확인하는 함수
	bool isInRange(int x, int y );

	//방문하려는 위치가 이미 방문했던 노드인지 확인
	bool hasVisited(int x, int y, float gCost);

	//탐색하려는 노드가 목표 노드인지 확인
	bool isDestination(const Node* node);

	bool isMovable(int x, int y);

private:
	//열린 리스트 (방문할 노드)
	std::vector<Node*> _openList;

	//닫힌 리스트 (방문한 노드)
	std::vector<Node*> _closedList;

	//시작 노드
	Node* _startNode;

	//목표 노드
	Node* _goalNode;

	//맵 복사 데이터
	std::vector<uint8>* _mapData;
};