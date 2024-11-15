#pragma once
#include"../../UIBase.h"

class GameOver :public UIBase
{
public:
	GameOver()          {};
	~GameOver()override {};

	void Update()override;
	void Init()  override;

	void SetChangeAlpha(float _change) { m_alphaChange = _change; }

	const float& GetAlphaChange()const { return m_alphaChange; }

private:
	float m_alpha = 0.0f;
	float m_alphaChange = 0.05f;
};