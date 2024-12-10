#include "Select.h"
#include"../../../../main.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../ObjectManager.h"
#include"../Cursor/Cursor.h"
#include"../Game/Game.h"
#include"../Exit/Exit.h"

void Select::Update()
{
	if (!SceneManager::Instance().GetBlackAlphaFlg())
	{
		Math::Vector2 _gamePos = m_ObjectManager.lock()->GetGame().lock()->GetVector2Pos();
		Math::Vector2 _exitPos = m_ObjectManager.lock()->GetExit().lock()->GetVector2Pos();
		float _gameHarfHeight = m_ObjectManager.lock()->GetGame().lock()->GetHeight() / 2.0f;
		float _gameHarfWidth = m_ObjectManager.lock()->GetGame().lock()->GetWidth() / 2.0f;
		float _exitHarfHeight = m_ObjectManager.lock()->GetExit().lock()->GetHeight() / 2.0f;
		float _exitHarfWidth = m_ObjectManager.lock()->GetExit().lock()->GetWidth() / 2.0f;
		Math::Vector2 _cursorPos = m_ObjectManager.lock()->GetCursor().lock()->GetVector2Pos();

		if (m_pos != _gamePos &&
			_cursorPos.x <= _gamePos.x + _gameHarfWidth && _cursorPos.x >= _gamePos.x - _gameHarfWidth &&
			_cursorPos.y <= _gamePos.y + _gameHarfHeight && _cursorPos.y >= _gamePos.y - _gameHarfHeight)
		{
			m_pos = _gamePos;
			KdAudioManager::Instance().Play("Asset/Sound/Title/SE/キャンセル1.WAV", 0.05f, false);
		}
		if (m_pos != _exitPos &&
			_cursorPos.x <= _exitPos.x + _exitHarfWidth && _cursorPos.x >= _exitPos.x - _exitHarfWidth &&
			_cursorPos.y <= _exitPos.y + _exitHarfHeight && _cursorPos.y >= _exitPos.y - _exitHarfHeight)
		{
			m_pos = _exitPos;
			KdAudioManager::Instance().Play("Asset/Sound/Title/SE/キャンセル1.WAV", 0.05f, false);
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (_cursorPos.x <= _gamePos.x + _gameHarfWidth && _cursorPos.x >= _gamePos.x - _gameHarfWidth &&
				_cursorPos.y <= _gamePos.y + _gameHarfHeight && _cursorPos.y >= _gamePos.y - _gameHarfHeight)
			{
				KdAudioManager::Instance().Play("Asset/Sound/Title/SE/決定ボタンを押す33.WAV", 0.05f, false);
				SceneManager::Instance().BlackAlphaChange(0.01f, true);
			}
			else if (_cursorPos.x <= _exitPos.x + _exitHarfWidth && _cursorPos.x >= _exitPos.x - _exitHarfWidth &&
				_cursorPos.y <= _exitPos.y + _exitHarfHeight && _cursorPos.y >= _exitPos.y - _exitHarfHeight)
			{
				Application::Instance().End();
			}
		}
	}
	else
	{
		if (SceneManager::Instance().GetBlackAlpha() >= 1.0f)
		{
			SceneManager::Instance().SetNextScene
			(
				SceneManager::SceneType::Game
			);
		}
	}

	m_SingleAngle += 2.0f;
	if (m_SingleAngle > 360.0f)m_SingleAngle -= 360.0f;
	m_alpha = (sin(DirectX::XMConvertToRadians(m_SingleAngle))) + 1.5f;

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
	m_alpha = m_MaxAlpha;
	m_color = { 1.0f,1.0f,1.0f,m_alpha };
	m_rect = { 0,0,long(520),long(141) };
	m_size = m_MaxSize;
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Select/Particle_Soft.png");
}
