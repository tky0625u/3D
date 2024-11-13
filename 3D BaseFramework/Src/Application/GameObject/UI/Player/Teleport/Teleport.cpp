#include "Teleport.h"
#include"../../../Character/Player/Player.h"

void Teleport::Update()
{
	if (m_player.lock()->GetTeleportFlg())
	{
		if (m_alpha < 1.0f)m_alpha += 0.05f;
	}
	else
	{
		if (m_alpha > 0.0f)m_alpha -= 0.05f;
	}

	m_color = { 1,1,1,m_alpha };
}

void Teleport::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
}

void Teleport::Init()
{
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Player/Teleport/cooltext469913006273259.png");
	m_pos = { 0.0f,-300.0f };
	m_size = 1.0f;
	m_alpha = 0.0f;
	m_color = { 1,1,1,m_alpha };
	m_rect = { 0,0,396,118 };
}
