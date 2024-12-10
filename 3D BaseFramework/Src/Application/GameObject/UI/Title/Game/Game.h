#pragma once
#include"../../UIBase.h"

class Game :public UIBase
{
public:
	Game() {};
	~Game()override {}

	void DrawSprite()override;
	void Init()override;

	const float& GetHeight()const { return m_rect.height * m_size; }
	const float& GetWidth() const { return m_rect.width * m_size; }

private:

};