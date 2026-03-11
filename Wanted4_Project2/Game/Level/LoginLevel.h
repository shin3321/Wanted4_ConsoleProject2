#pragma once
#include "Level\Level.h"
#include "Math/Color.h"

#include <string>

using namespace Wanted;
constexpr int MAX_ID_LENGTH = 15;

class LoginLevel : public Level
{
	RTTI_DECLARATIONS(LoginLevel, Level)

public:
	LoginLevel();
	~LoginLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void UpdateId();

	bool _isRunnig = true;
private:

	std::string inputId = "";
};

