#pragma once
#include"../../UIBase.h"

class Title :public UIBase
{
public:
	Title() {};
	~Title()override {}

	void DrawSprite()override;
	void Init()override;

	void SetPos(Math::Vector2 _pos) { m_pos = _pos; }

	const Math::Vector2 GetVector2Pos()const { return m_pos; }

private:

};