#include "Select.h"

// シーンマネジャ
#include"../../../../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"../../../ObjectManager.h"
// カーソル
#include"../Cursor/Cursor.h"
// GameUI
#include"../Game/Game.h"
// ExitUI
#include"../Exit/Exit.h"

void Select::Update()
{
	// GameUI
	// 座標
	Math::Vector2 _gamePos = m_ObjectManager.lock()->GetGame().lock()->GetVector2Pos();
	// 縦幅
	float _gameHarfHeight = m_ObjectManager.lock()->GetGame().lock()->GetHeight() / 2.0f;
	// 横幅
	float _gameHarfWidth = m_ObjectManager.lock()->GetGame().lock()->GetWidth() / 2.0f;
	
	// ExitUI
	// 座標
	Math::Vector2 _exitPos = m_ObjectManager.lock()->GetExit().lock()->GetVector2Pos();
	// 縦幅
	float _exitHarfHeight = m_ObjectManager.lock()->GetExit().lock()->GetHeight() / 2.0f;
	// 横幅
	float _exitHarfWidth = m_ObjectManager.lock()->GetExit().lock()->GetWidth() / 2.0f;
	
	// カーソル座標
	Math::Vector2 _cursorPos = m_ObjectManager.lock()->GetCursor().lock()->GetVector2Pos();

	if (!SceneManager::Instance().GetBlackAlphaFlg()) // 暗転フラグがOFFだったら
	{
		// カーソルがGameUIに重なったら
		if (_cursorPos.x <= _gamePos.x + _gameHarfWidth && _cursorPos.x >= _gamePos.x - _gameHarfWidth &&
			_cursorPos.y <= _gamePos.y + _gameHarfHeight && _cursorPos.y >= _gamePos.y - _gameHarfHeight)
		{
			// GameUIの座標に移動
			m_pos = _gamePos;

			// 選択中フラグがOFFだったら効果音を鳴らす
			if (!m_SelectFlg)KdAudioManager::Instance().Play("Asset/Sound/Title/SE/キャンセル1.WAV", 0.05f, false);

			// 選択中フラグON
			m_SelectFlg = true;
		}
		// カーソルがExitUIに重なったら
		else if (_cursorPos.x <= _exitPos.x + _exitHarfWidth && _cursorPos.x >= _exitPos.x - _exitHarfWidth &&
			     _cursorPos.y <= _exitPos.y + _exitHarfHeight && _cursorPos.y >= _exitPos.y - _exitHarfHeight)
		{
			// ExitUIの座標に移動
			m_pos = _exitPos;

			// 選択中フラグがOFFだったら効果音を鳴らす
			if (!m_SelectFlg)KdAudioManager::Instance().Play("Asset/Sound/Title/SE/キャンセル1.WAV", 0.05f, false);

			// 選択中フラグON
			m_SelectFlg = true;
		}
		else // どちらとも重なっていなかったら
		{
			// 透明
			m_alpha = 0.0f;

			// 選択中フラグOFF
			m_SelectFlg = false;
		}
	}

	// 選択中フラグがONだったら
	if (m_SelectFlg)
	{
		// sinカーブ用角度増加
		m_SingleAngle += m_ChangeAngle;
		// 360度より大きかったら-360度をする
		if (m_SingleAngle > 360.0f)m_SingleAngle -= 360.0f;
		// アルファ値変更 (sinカーブ)
		m_alpha = (sin(DirectX::XMConvertToRadians(m_SingleAngle))) + m_AlphaCorrection;
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void Select::DrawSprite()
{
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Add);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Alpha);
}

void Select::Init()
{
	m_pos = m_ObjectManager.lock()->GetGame().lock()->GetVector2Pos();
	m_color = { 1.0f,1.0f,1.0f,m_alpha };
	m_rect = { 0,0,long(520),long(141) };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Select/Particle_Soft.png");
}
