#include "Enemy_HP.h"
#include"../../../Camera/GameCamera/GameCamera.h"
#include"../../../Character/Enemy/EnemyBase.h"
#include"../../../Character/Player/Player.h"
#include"../../../Character/Action/Player/Player_ConText.h"

void Enemy_HP::Update()
{
	if (m_camera.expired())return;
	if (m_target.expired())
	{
		if (!IsExpired())Expired();
		return;
	}

	if (m_rect[HP::hp].width > m_rect[HP::down].width)m_rect[HP::down].width = m_rect[HP::hp].width;

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

	//座標変換
	Math::Vector3 _pos = { m_pos.x,m_pos.y,0.0f };
	m_camera.lock()->WorkCamera()->ConvertWorldToScreenDetail(m_target.lock()->GetHPMat().Translation(), _pos);

	if (_pos.z >= 0.0f)
	{
		if (!m_target.lock()->GetLockONFlg() && m_target.lock()->GetTarget().lock()->GetConText()->GetLockONFlg()) { m_alpha = 0.1f; }
		else
		{
			m_alpha = 1.0f;
		}
	}
	else
	{
		m_alpha = -1.0f;
	}

	m_pos = { _pos.x,_pos.y };
	m_color = { 1,1,1,m_alpha };
}

void Enemy_HP::DrawSprite()
{
	for (int h = 0; h < HP::num; ++h)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex[h], (int)(m_pos.x - (m_MaxWidth / 2)), (int)m_pos.y, (int)m_rect[h].width * m_size, (int)m_rect[h].height * m_size, &m_rect[h], &m_color, m_pivot);
	}
}

void Enemy_HP::Init()
{
	m_pivot = { 0.0f,0.5f };
	m_MaxWidth = 200;
	m_alpha = 1.0f;
	m_color = { 1,1,1,m_alpha };
	if (m_target.expired())return;
	m_MaxHP = m_target.lock()->GetParam().Hp;
	m_beforeHP = m_MaxHP;

	//HPバー
	m_rect[HP::hp] = { 0,0,m_MaxWidth,10 };
	m_pTex[HP::hp] = std::make_shared<KdTexture>();
	m_pTex[HP::hp]->Load("Asset/Textures/UI/Enemy/Enemy_HP.png");

	//HPボックス
	m_rect[HP::box] = { 0,0,m_MaxWidth,10 };
	m_pTex[HP::box] = std::make_shared<KdTexture>();
	m_pTex[HP::box]->Load("Asset/Textures/UI/Enemy/Enemy_HPBox.png");

	//HP減少ゲージ
	m_rect[HP::down] = { 0,0,m_MaxWidth,10 };
	m_pTex[HP::down] = std::make_shared<KdTexture>();
	m_pTex[HP::down]->Load("Asset/Textures/UI/Enemy/Enemy_HPDown.png");
}
