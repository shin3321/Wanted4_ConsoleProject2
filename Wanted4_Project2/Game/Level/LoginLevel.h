#pragma once
#include "Level\ManageLevel.h"
#include "Math/Color.h"

#include <string>

using namespace Wanted;
constexpr int MAX_ID_LENGTH = 15;

class LoginLevel : public ManageLevel
{
	RTTI_DECLARATIONS(LoginLevel, ManageLevel)

public:
	LoginLevel();
	~LoginLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void UpdateId();

private:

	std::string inputId = "";
};

