﻿#include "Player_HP.h"
#include"../../../Character/Player/Player.h"

void Player_HP::Update()
{
	if (m_target.expired())return;

	//HP減少
	if (m_target.lock()->GetParam().Hp != m_beforeHP)
	{
		m_rect[HP::hp] = { 0,0,(m_MaxWidth / m_MaxHP) * m_target.lock()->GetParam().Hp,10 };
		m_beforeHP = m_target.lock()->GetParam().Hp;
		m_DownTime = 60;
	}

	if (m_DownTime > 0)m_DownTime--;
	else
	{
		m_DownTime = 0;
		if (m_rect[HP::down].width != m_rect[HP::hp].width)m_rect[HP::down].width--; //現在のHPまで徐々に減少
	}
}

void Player_HP::DrawSprite()
{
	for (int h = 0; h < HP::num; ++h)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex[h], (int)m_pos.x, (int)m_pos.y, (int)m_rect[h].width, (int)m_rect[h].height, &m_rect[h], &m_color, m_pivot);
	}
}

void Player_HP::Init()
{
	m_pos = { -600.0f,340.0f };
	m_pivot = { 0.0f,0.5f };
	m_MaxWidth = 800;
	m_color = { 1,1,1,1 };
	if (m_target.expired())return;
	m_MaxHP = m_target.lock()->GetParam().Hp;
	m_beforeHP = m_MaxHP;

	//HPバー
	m_rect[HP::hp] = { 0,0,m_MaxWidth,10 };
	m_pTex[HP::hp] = std::make_shared<KdTexture>();
	m_pTex[HP::hp]->Load("Asset/Textures/UI/Player/HP/Player_HP.png");

	//HPボックス
	m_rect[HP::box] = { 0,0,m_MaxWidth,10 };
	m_pTex[HP::box] = std::make_shared<KdTexture>();
	m_pTex[HP::box]->Load("Asset/Textures/UI/Player/HP/Player_HPBox.png");

	//HP減少ゲージ
	m_rect[HP::down] = { 0,0,m_MaxWidth,10 };
	m_pTex[HP::down] = std::make_shared<KdTexture>();
	m_pTex[HP::down]->Load("Asset/Textures/UI/Player/HP/Player_HPDown.png");
}