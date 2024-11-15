#pragma once
#include"../../UIBase.h"

class Game :public UIBase
{
public:
	Game() {};
	~Game()override {}

	void DrawSprite()override;
	void Init()override;

private:

};