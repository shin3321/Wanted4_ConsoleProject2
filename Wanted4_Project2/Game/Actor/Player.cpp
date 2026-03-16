#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Render/Renderer.h"

#include "Level/Game.h"
#include "Header/Packet.h"
#include "Network/INetwork.h"
#include "Actor/Unit.h"

#include <algorithm>
#include <iostream>

Player::Player()
{
	// 타이머 목표 시간 설정.
	//timer.SetTargetTime(fireInterval);
}

Player::Player(uint16_t id)
	:_id(id)
{
}

Player::~Player()
{
 }

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 종료 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 게임 종료.
		QuitGame();
	}

	// 왼쪽 클릭 
	if (Input::Get().GetMouseButtonDown(0))
	{
		UpdateDragSelection(Input::Get().MousePosition());
	}

	//왼쪽 누름 -> 드래그 시작
	if (Input::Get().GetMouseButton(0) && _isDragging)
	{
		_dragEnd = Input::Get().MousePosition();
		RenderDragBox();
	}

	// 왼쪽 뗌 -> 드래그 끝 
	if (Input::Get().GetMouseButtonUp(0))
	{
		FinishDragSelection(Input::Get().MousePosition());
	}

	// 왼쪽 클릭 && 선택된 유닛이 있다면 - 이동 명령
	if (Input::Get().GetMouseButtonDown(0) && !_selectedUnitIds.empty())
	{
		MoveUnit(Input::Get().MousePosition());
	}

	// 우클릭 -> 유닛 생성 - 클릭하는 순간 1번만
	if (Input::Get().GetMouseButtonDown(1))
	{
		_selectedUnitIds.clear();
		SpawnUnit(Input::Get().MousePosition());
	}
}

void Player::UpdateDragSelection(Vector2 mousePos)
{
	for (auto& [unitId, unit] : _selectedUnitIds)
	{
		unit->Unselect();
	}
	_dragStart = mousePos;
	_isDragging = true;

}

void Player::FinishDragSelection(Vector2 mousePos)
{
	_isDragging = false;

	//전체 유닛 순회
	for (auto& [id, unit] : Game::Get().GetUnits())
	{
		// 내 유닛이 아니면 스킵
		if (unit->_ownerId != _id) continue;

		// 드래그 범위 안에 있는지 체크
		if (unit->_pos.x >= minX && unit->_pos.x <= maxX &&
			unit->_pos.y >= minY && unit->_pos.y <= maxY)
		{
			unit->Select();
			_selectedUnitIds.insert({ id, unit });
		}
	}
}

void Player::SpawnUnit(Vector2 mousePos)
{
	Packet unitPacket;
	unitPacket.write<uint16_t>(0);
	unitPacket.write<uint16_t>(PK_CS_SPAWN_UNIT);
	unitPacket.write<uint16_t>(_id);
	unitPacket.write<Vector2>(mousePos);
	uint16_t packetSize = static_cast<uint16_t>(unitPacket.size());
	uint16_t networkSize = htons(packetSize);
	memcpy(&unitPacket.getBuffer()[0], &networkSize, sizeof(uint16_t));
	_networker->DoSend(unitPacket.getBuffer().data(), packetSize);
}

void Player::MoveUnit(Vector2 mousePos)
{
	if (_selectedUnitIds.empty()) return;

	

	for (auto&[ unitId, unit]: _selectedUnitIds)
	{
		unit->
	}
	Packet unitMovePacket;
	unitMovePacket.write<uint16_t>(0);
	unitMovePacket.write<uint16_t>(PK_CS_MOVE_UNIT);
	unitMovePacket.write<uint16_t>(_id);
	unitMovePacket.write<Vector2>(mousePos);
	unitMovePacket.write<uint16_t>(static_cast<uint16_t>(_selectedUnitIds.size()));
	uint16_t packetSize = static_cast<uint16_t>(unitMovePacket.size());
	uint16_t networkSize = htons(packetSize);
	memcpy(&unitMovePacket.getBuffer()[0], &networkSize, sizeof(uint16_t));

	_networker->DoSend(unitMovePacket.getBuffer().data(), packetSize);
	_selectedUnitIds.clear();
}

void Player::RenderDragBox()
{
	minX = (std::min)(_dragStart.x, _dragEnd.x);
	minY = (std::min)(_dragStart.y, _dragEnd.y);
	maxX = (std::max)(_dragStart.x, _dragEnd.x);
	maxY = (std::max)(_dragStart.y, _dragEnd.y);

	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			if (x == minX || x == maxX || y == minY || y == maxY)
				Renderer::Get().Submit(".", Vector2(x, y), Color::Purple, 5);
		}
	}
}

void Player::AddUnit(uint16_t unitId)
{
	_myUnitIds.insert(unitId);
}
