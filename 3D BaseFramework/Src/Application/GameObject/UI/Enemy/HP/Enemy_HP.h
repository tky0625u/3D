﻿#pragma once
#include"../../UIBase.h"

class TPSCamera;
class EnemyBase;

class Enemy_HP :public UIBase
{
public:
	Enemy_HP() {}
	~Enemy_HP()       override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	void SetTarget(std::shared_ptr<EnemyBase> _target) { m_target = _target; }
	void SetCamera(std::shared_ptr<TPSCamera> _camera) { m_camera = _camera; }

private:

	enum HP
	{
		box,
		down,
		hp,
		num
	};

	std::weak_ptr<TPSCamera>  m_camera;
	std::weak_ptr<EnemyBase>   m_target;
	std::shared_ptr<KdTexture> m_pTex[HP::num];
	Math::Rectangle            m_rect[HP::num];
	int                        m_MaxHP = 0;
	int                        m_beforeHP = 0;
	long                       m_MaxWidth = 0.0l;
	int                        m_DownTime = 0;
	float                      m_alpha = 1.0f;
};