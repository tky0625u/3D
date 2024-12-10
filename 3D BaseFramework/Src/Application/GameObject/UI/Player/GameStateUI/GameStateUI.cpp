#include "GameStateUI.h"

void GameStateUI::Update()
{
	m_state->StateUpdate(shared_from_this());
}

void GameStateUI::GameOver::Enter(std::shared_ptr<GameStateUI> owner)
{
	m_alpha = 0.0f;
	owner->m_pTex = std::make_shared<KdTexture>();
	owner->m_pTex->Load("Asset/Textures/UI/Player/GameStateUI/GameOver/Dead.png");
	owner->m_color = { 1,1,1,m_alpha };
	owner->m_rect  = { 0,0,1280,107 };
	owner->m_flow = GameStateUI::Flow::UpdateType;
	return;
}

void GameStateUI::GameOver::Update(std::shared_ptr<GameStateUI> owner)
{
	if (m_alpha < 1.0f)
	{
		m_alpha += owner->m_alphaChange;
	}
	else if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
	}

	owner->m_color = { 1,1,1,m_alpha };
}

void GameStateUI::GameOver::Exit(std::shared_ptr<GameStateUI> owner)
{
}

void GameStateUI::Clear::Enter(std::shared_ptr<GameStateUI> owner)
{
	m_alpha = 0.0f;
	owner->m_pTex = std::make_shared<KdTexture>();
	owner->m_pTex->Load("Asset/Textures/UI/Player/GameStateUI/Clear/Congratulations.png");
	owner->m_color = { 1,1,1,m_alpha };
	owner->m_rect = { 0,0,1280,107 };
	owner->m_flow = GameStateUI::Flow::UpdateType;
	return;
}

void GameStateUI::Clear::Update(std::shared_ptr<GameStateUI> owner)
{
	if (m_alpha < 1.0f)
	{
		m_alpha += owner->m_alphaChange;
	}
	else if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
	}

	owner->m_color = { 1,1,1,m_alpha };
}

void GameStateUI::Clear::Exit(std::shared_ptr<GameStateUI> owner)
{
}