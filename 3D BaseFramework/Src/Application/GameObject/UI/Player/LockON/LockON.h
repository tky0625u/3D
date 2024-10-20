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

private:
	std::weak_ptr<Player>      m_target;
	int                        m_beforeTargetID = -1;
	float                      m_alpha = 1.0f;
	float                      m_size = 1.0f;
};