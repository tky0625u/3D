#include "GameOver.h"

void GameOver::Update()
{
	if (GetAsyncKeyState('Y') & 0x8000)m_alpha = 0.0f;

	if (m_alpha < 1.0f)
	{
		m_alpha += m_alphaChange;
	}
	else if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
	}

	m_color = { 1,1,1,m_alpha };
}

void GameOver::Init()
{
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Player/GameOver/Death.png");
	m_color = { 1,1,1,m_alpha };
	m_rect = { 0,0,1280,107 };

}
