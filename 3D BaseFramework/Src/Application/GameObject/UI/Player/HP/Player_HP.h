#pragma once
#include"../../UIBase.h"

class Player;

class Player_HP :public UIBase
{
public:
	Player_HP()                {};
	~Player_HP()      override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	void SetTraget(std::shared_ptr<Player> _target) { m_target = _target; }

private:

	enum HP
	{
		box,
		down,
		hp,
		num
	};

	std::weak_ptr<Player>      m_target;
	std::shared_ptr<KdTexture> m_pTex[HP::num];
	Math::Rectangle            m_rect[HP::num];
	int                        m_MaxHP = 0;
	int                        m_beforeHP = 0;
	long                       m_MaxWidth = 0.0l;
	int                        m_DownTime = 0;
};