#include "Title.h"

void Title::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
}

void Title::Init()
{
	m_color = { 1.0f,1.0f,1.0f,1.0f };
	m_rect = { 0,0,long(851),long(121) };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Title/Cool Text - Gradivus Tower 468850688032583.png");
}
