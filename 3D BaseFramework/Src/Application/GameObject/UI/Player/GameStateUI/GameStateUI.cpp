#include "GameStateUI.h"

void GameStateUI::Update()
{
	// ステート更新
	m_state->StateUpdate(shared_from_this());
}

// セッター========================================================================================
// アルファ値変化量
void GameStateUI::SetChangeAlpha(float _change)
{
	m_alphaChange = _change;
}

// ステート
void GameStateUI::SetState(bool _IsClear)
{
	if (_IsClear)
	{
		m_state = std::make_shared<Clear>();
		m_flow = Flow::EnterType;
	}
	else
	{
		m_state = std::make_shared<GameOver>();
		m_flow = Flow::EnterType;
	}
}
//=================================================================================================

// ゲッター========================================================================================
// アルファ値変化量
const float& GameStateUI::GetAlphaChange() const
{
	return m_alphaChange;
}
//=================================================================================================

// ステート更新
void GameStateUI::StateBase::StateUpdate(std::shared_ptr<GameStateUI> owner)
{
	switch (owner->m_flow)
	{
	case Flow::EnterType:
		Enter(owner);
		break;
	case Flow::UpdateType:
		Update(owner);
		break;
	case Flow::ExitType:
		Exit(owner);
		break;
	default:
		break;
	}
}

// ゲームオーバー==================================================================================
void GameStateUI::GameOver::Enter(std::shared_ptr<GameStateUI> owner)
{
	// アルファ値
	m_alpha = 0.0f;

	// テクスチャ
	owner->m_pTex = std::make_shared<KdTexture>(); // 生成
	owner->m_pTex->Load("Asset/Textures/UI/Player/GameStateUI/GameOver/Dead.png"); // 読み込み
	
	// 色
	owner->m_color = { 1,1,1,m_alpha };
	
	// 切り取り範囲
	owner->m_rect  = { 0,0,1280,107 };
	
	// 次のステートへ
	owner->m_flow = GameStateUI::Flow::UpdateType;
	return;
}

void GameStateUI::GameOver::Update(std::shared_ptr<GameStateUI> owner)
{
	if (m_alpha < 1.0f) // アルファ値が1.0より下だったら
	{
		m_alpha += owner->m_alphaChange; // アルファ値増加
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
//=================================================================================================

// クリア==========================================================================================
void GameStateUI::Clear::Enter(std::shared_ptr<GameStateUI> owner)
{
	// アルファ値
	m_alpha = 0.0f;

	// テクスチャ
	owner->m_pTex = std::make_shared<KdTexture>(); // 生成
	owner->m_pTex->Load("Asset/Textures/UI/Player/GameStateUI/Clear/Congratulations.png"); // 読み込み
	
	// 色
	owner->m_color = { 1,1,1,m_alpha };
	
	// 切り取り範囲
	owner->m_rect = { 0,0,1280,107 };
	
	// 切り取り範囲
	owner->m_flow = GameStateUI::Flow::UpdateType;
	return;
}

void GameStateUI::Clear::Update(std::shared_ptr<GameStateUI> owner)
{
	if (m_alpha < 1.0f) // アルファ値が1.0より下だったら
	{
		m_alpha += owner->m_alphaChange; // アルファ値増加
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
//=================================================================================================