#pragma once

// UI基底
#include"../../UIBase.h"

class Exit :public UIBase
{
public:
	Exit() {};
	~Exit()override {}

	void DrawSprite()override;
	void Init()override;

	// ゲッター================================================================
	const float& GetHeight()const { return m_rect.height * m_size; } // 縦幅
	const float& GetWidth()const  { return m_rect.width * m_size; }  // 横幅
	//=========================================================================

private:

};