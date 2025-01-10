#include "Player_Stamina.h"

// プレイヤー
#include"../../../Character/Player/Player.h"

void Player_Stamina::Update()
{
	if (m_target.expired())return; // ターゲットが消滅していたら早期リターン

	// スタミナ減少
	if (m_target.lock()->GetParam().Sm != m_beforeStamina) // 前回のスタミナと現在のスタミナが違う場合
	{//                    最大幅     最大スタミナ            現在のスタミナ
		float stamina = (m_MaxWidth / m_MaxStamina) * m_target.lock()->GetParam().Sm; // 幅計算
		m_rect[Stamina::stamina] = { 0,0,long(stamina),64 };
		m_beforeStamina = m_target.lock()->GetParam().Sm; // 前回のスタミナを更新
	}
}

void Player_Stamina::DrawSprite()
{
	for (int s = 0; s < Stamina::num; ++s)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex[s], (int)m_pos.x, (int)m_pos.y, (int)m_rect[s].width * m_size, (int)m_rect[s].height * m_size, &m_rect[s], &m_color, m_pivot);
	}
}

void Player_Stamina::Init()
{
	m_pivot = { 0.0f,0.5f };
	m_MaxWidth = 1024;
	m_color = { 1,1,1,1 };
	if (m_target.expired())return;
	m_MaxStamina = m_target.lock()->GetParam().Sm;
	m_beforeStamina = m_MaxStamina;

	//スタミナバー
	m_rect[Stamina::stamina] = { 0,0,long(m_MaxWidth),64 };
	m_pTex[Stamina::stamina] = std::make_shared<KdTexture>();
	m_pTex[Stamina::stamina]->Load("Asset/Textures/UI/Player/Stamina/Player_Stamina_line.png");

	//スタミナボックス
	m_rect[Stamina::box] = { 0,0,long(m_MaxWidth),64 };
	m_pTex[Stamina::box] = std::make_shared<KdTexture>();
	m_pTex[Stamina::box]->Load("Asset/Textures/UI/Player/Stamina/frame.png");
}
