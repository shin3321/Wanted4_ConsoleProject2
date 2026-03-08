#pragma once
#include "Actor\Actor.h"
#include "Utils/Timer.h"
#include <iostream>

using namespace Wanted;

class Unit : public Actor
{
	RTTI_DECLARATIONS(Unit, Actor)

public:  
    Unit();
    Unit(Vector2 pos, uint16_t ownerId);
    Unit(Vector2 pos, Color color);
    ~Unit();

    void Select();
    void Unselect();

    uint16_t _id;
    uint16_t _ownerId = -1;

    //위치
    Vector2 _pos;

    //// 체력 (HP바 표시용)
    //int _hp;
    //int _maxHp;

    // 선택 여부 (드래그 선택)
    bool _isSelected = false;

    // 렌더링
  //  char _symbol = 'O';     // 내 유닛
};

