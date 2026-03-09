#pragma once
#include "Level\ManageLevel.h"
#include "Math/Color.h"

using namespace Wanted;


class WaitingRoomLevel :   public ManageLevel
{

	RTTI_DECLARATIONS(WaitingRoomLevel, ManageLevel)

public:
	WaitingRoomLevel();
	~WaitingRoomLevel();

private:

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
private:
	//현재 활성화된 메뉴 아이템 인덱스
	int currentIndex = 0;

	//선택된 아이템의 색상
	Color selectedColor = Color::Green;

	//선택되지 않은 아이템의 색상
	Color unselectedColor = Color::White;

	//메뉴 아이템 배열
	std::vector<MenuItem*> items;

};

