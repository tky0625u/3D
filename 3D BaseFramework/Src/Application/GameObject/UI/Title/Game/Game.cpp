#include "Game.h"

void Game::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
}

void Game::Init()
{
	m_pivot = { 0.5f,0.5f };
	m_color = { 1.0f,1.0f,1.0f,1.0f };
	m_rect = { 0,0,long(352),long(111) };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Game/Cool Text - GAME 468884988188424.png");
}

// ゲッター========================================================================================
// 縦幅
const float& Game::GetHeight() const
{
	return m_rect.height * m_size;
}

// 横幅
const float& Game::GetWidth() const
{
	return m_rect.width * m_size;
}
//=================================================================================================