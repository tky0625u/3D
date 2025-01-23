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
	const float& GetHeight()const { return m_rect.height * m_size; } // 縦幅
	const float& GetWidth()const  { return m_rect.width * m_size; }  // 横幅
	//===================================================================================

private:

};