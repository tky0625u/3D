#pragma once
#include"../../UIBase.h"

class Player;

class LockON :public UIBase
{
public:
	LockON() {};
	~LockON()override {};

	void Update()override;
	void DrawSprite()override;
	void Init()override;

	void SetTraget(std::shared_ptr<Player> _target) { m_target = _target; }
	void SetChangeAlpha(float _change)              { m_alphaChange = _change; }
	void SetChangeSize(float _change)               { m_sizeChange = _change; }

	const float& GetChangeSize()const  { return m_sizeChange; }
	const float& GetChangeAlpha()const { return m_alphaChange; }

private:
	std::weak_ptr<Player>      m_target;
	int                        m_beforeTargetID = -1;
	float                      m_alpha = 1.0f;
	float                      m_alphaChange = 0.05f;
	float                      m_size = 1.0f;
	float                      m_sizeChange = 0.05f;
};