#include "Enemy_UI.h"
#include"../../Camera/CameraBase.h"
#include"../../Character/Enemy/EnemyBase.h"

void Enemy_UI::Update()
{
	if (m_camera.expired())return;
	if (m_target.expired())return;

	//HP減少
	if (m_target.lock()->GetParam().Hp != m_beforeHP)
	{
		m_HpRect = { 0,0,(m_MaxWidth / m_MaxHP) * m_target.lock()->GetParam().Hp,10 };
		m_beforeHP = m_target.lock()->GetParam().Hp;
		m_DownTime = 120;
	}

	if (m_DownTime > 0)m_DownTime--;
	else
	{
		m_DownTime = 0;
		if (m_HpDownRect.width != m_HpRect.width)m_HpDownRect.width--; //現在のHPまで徐々に減少
	}

	//座標変換
	Math::Vector3 _pos = { m_pos.x,m_pos.y,0.0f };
	m_camera.lock()->WorkCamera()->ConvertWorldToScreenDetail(m_target.lock()->GetHPMat().Translation(), _pos);
	if (_pos.z >= 0.0f)
	{
	m_pos = { _pos.x,_pos.y };
	}
}

void Enemy_UI::DrawSprite()
{
	//HPボックス
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_HpBoxTex, (int)(m_pos.x - (m_MaxWidth / 2)), (int)m_pos.y, (int)m_HpBoxRect.width, (int)m_HpBoxRect.height, &m_HpBoxRect, &m_color, m_pivot);
	
	//HP減少ゲージ
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_HpDownTex, (int)(m_pos.x - (m_MaxWidth / 2)), (int)m_pos.y, (int)m_HpDownRect.width, (int)m_HpDownRect.height, &m_HpDownRect, &m_color, m_pivot);

	//HPバー
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_HpTex, (int)(m_pos.x - (m_MaxWidth / 2)), (int)m_pos.y, (int)m_HpRect.width, (int)m_HpRect.height, &m_HpRect, &m_color, m_pivot);
}

void Enemy_UI::Init()
{
	//HPバー
	m_pos       = Math::Vector2::Zero;
	m_pivot     = { 0.0f,0.5f };
	m_MaxWidth  = 200;
	m_HpRect    = { 0,0,m_MaxWidth,10 };
	m_color     = { 1,1,1,1 };
	m_HpTex     = std::make_shared<KdTexture>();
	m_HpTex->Load("Asset/Textures/UI/Enemy/Enemy_HP.png");
	if (m_target.expired())return;
	m_MaxHP     = m_target.lock()->GetParam().Hp;
	m_beforeHP  = m_MaxHP;

	//HPボックス
	m_HpBoxTex  = std::make_shared<KdTexture>();
	m_HpBoxTex->Load("Asset/Textures/UI/Enemy/Enemy_HPBox.png");
	m_HpBoxRect = m_HpRect;

	//HP減少ゲージ
	m_HpDownTex = std::make_shared<KdTexture>();
	m_HpDownTex->Load("Asset/Textures/UI/Enemy/Enemy_HPDown.png");
	m_HpDownRect = m_HpRect;
}
