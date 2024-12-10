#pragma once
#include"../../UIBase.h"

class Exit :public UIBase
{
public:
	Exit() {};
	~Exit()override {}

	void DrawSprite()override;
	void Init()override;

	const float& GetHeight()const { return m_rect.height * m_size; }
	const float& GetWidth() const { return m_rect.width * m_size; }

private:

};