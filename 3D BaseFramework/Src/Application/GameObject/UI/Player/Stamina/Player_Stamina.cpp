#include "Player_Stamina.h"
#include"../../../Character/Player/Player.h"

void Player_Stamina::Update()
{
	if (m_target.expired())return;

	//スタミナ減少
	if (m_target.lock()->GetParam().Sm != m_beforeStamina)
	{
		m_rect[Stamina::stamina] = { 0,0,(m_MaxWidth / m_MaxStamina) * m_target.lock()->GetParam().Sm,10 };
		m_beforeStamina = m_target.lock()->GetParam().Sm;
	}
}

void Player_Stamina::DrawSprite()
{
	for (int s = 0; s < Stamina::num; ++s)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex[s], (int)m_pos.x, (int)m_pos.y, (int)m_rect[s].width, (int)m_rect[s].height, &m_rect[s], &m_color, m_pivot);
	}
}

void Player_Stamina::Init()
{
	m_pos = { -600.0f,325.0f };
	m_pivot = { 0.0f,0.5f };
	m_MaxWidth = 800;
	m_color = { 1,1,1,1 };
	if (m_target.expired())return;
	m_MaxStamina = m_target.lock()->GetParam().Sm;
	m_beforeStamina = m_MaxStamina;

	//スタミナバー
	m_rect[Stamina::stamina] = { 0,0,m_MaxWidth,10 };
	m_pTex[Stamina::stamina] = std::make_shared<KdTexture>();
	m_pTex[Stamina::stamina]->Load("Asset/Textures/UI/Player/Stamina/Player_Stamina.png");

	//スタミナボックス
	m_rect[Stamina::box] = { 0,0,m_MaxWidth,10 };
	m_pTex[Stamina::box] = std::make_shared<KdTexture>();
	m_pTex[Stamina::box]->Load("Asset/Textures/UI/Player/Stamina/Player_StaminaBox.png");
}
