#pragma once

// UI基底
#include"../../UIBase.h"

class Game :public UIBase
{
public:
	Game() {};
	~Game()override {}

	void DrawSprite()override;
	void Init()override;

	// ゲッター==========================================================================
	const float& GetHeight()const; // 縦幅
	const float& GetWidth()const;  // 横幅
	//===================================================================================

private:

};