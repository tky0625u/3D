#pragma once
#include"../../UIBase.h"

class Player;

class Player_Stamina :public UIBase
{
public:
	Player_Stamina()           {};
	~Player_Stamina() override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	void SetTraget(std::shared_ptr<Player> _target) { m_target = _target; }

private:

	enum Stamina
	{
		box,
		stamina,
		num
	};

	std::weak_ptr<Player>      m_target;
	std::shared_ptr<KdTexture> m_pTex[Stamina::num];
	Math::Rectangle            m_rect[Stamina::num];
	int                        m_MaxStamina    = 0;
	int                        m_beforeStamina = 0;
	float                      m_MaxWidth      = 0.0f;
};