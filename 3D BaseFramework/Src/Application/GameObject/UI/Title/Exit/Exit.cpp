#include "Exit.h"

void Exit::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
}

void Exit::Init()
{
	m_pivot = { 0.5f,0.5f };
	m_color = { 1.0f,1.0f,1.0f,1.0f };
	m_rect = { 0,0,long(285),long(107) };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Exit/Cool Text - EXIT 468886042904458.png");
}
