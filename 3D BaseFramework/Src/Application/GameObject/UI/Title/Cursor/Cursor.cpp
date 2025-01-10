#include "Cursor.h"

// メイン
#include"../../../../main.h"
// シーンマネジャ
#include"../../../../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"../../../ObjectManager.h"
// GameUI
#include"../Game/Game.h"
// ExitUI
#include"../Exit/Exit.h"
// パーティクル
#include"Particle/Particle.h"

void Cursor::Update()
{
	// 暗転フラグがOFFだったらマウスを動かせる
	if (!SceneManager::Instance().GetBlackAlphaFlg())
	{
		POINT _cursor;
		GetCursorPos(&_cursor);
		m_pos = { float(_cursor.x - 640.0f),float(-_cursor.y + 390.0f) };
	}
	// 画面が暗くなったらゲームへ
	else if (SceneManager::Instance().GetBlackAlpha() >= 1.0f)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		// クリック確認フラグがOFFだったら
		if (!m_ClickFlg)
		{
			// クリック判定
			// GameUI座標
			Math::Vector2 _gamePos = m_ObjectManager.lock()->GetGame().lock()->GetVector2Pos();
			// 縦幅
			float _gameHarfHeight = m_ObjectManager.lock()->GetGame().lock()->GetHeight() / 2.0f;
			// 横幅
			float _gameHarfWidth = m_ObjectManager.lock()->GetGame().lock()->GetWidth() / 2.0f;
			
			// ExitUI座標
			Math::Vector2 _exitPos = m_ObjectManager.lock()->GetExit().lock()->GetVector2Pos();
			// 縦幅
			float _exitHarfHeight = m_ObjectManager.lock()->GetExit().lock()->GetHeight() / 2.0f;
			// 横幅
			float _exitHarfWidth = m_ObjectManager.lock()->GetExit().lock()->GetWidth() / 2.0f;

			// カーソルがGameUIに重なっていたら
			if (m_pos.x <= _gamePos.x + _gameHarfWidth && m_pos.x >= _gamePos.x - _gameHarfWidth &&
				m_pos.y <= _gamePos.y + _gameHarfHeight && m_pos.y >= _gamePos.y - _gameHarfHeight)
			{
				KdAudioManager::Instance().Play("Asset/Sound/Title/SE/決定ボタンを押す33.WAV", 0.05f, false);
				// 暗転フラグON
				SceneManager::Instance().BlackAlphaChange(0.01f, true);
			}
			// カーソルがExitUIに重なっていたら
			else if (m_pos.x <= _exitPos.x + _exitHarfWidth && m_pos.x >= _exitPos.x - _exitHarfWidth &&
				m_pos.y <= _exitPos.y + _exitHarfHeight && m_pos.y >= _exitPos.y - _exitHarfHeight)
			{
				KdAudioManager::Instance().Play("Asset/Sound/Title/SE/決定ボタンを押す33.WAV", 0.05f, false);
				// ゲームを終了
				Application::Instance().End();
			}

			// パーティクル生成
			for (int p = 0; p < m_particle.ParticleNum; ++p)
			{
				// 生成
				std::shared_ptr<Particle> _particle = std::make_shared<Particle>();
				
				// 初期化
				_particle->Init();
			
				// パラメータセット
				// 移動量
				Math::Vector2 _move = { (rand() %  m_particle.MoveXNum + m_particle.MinMove.x + 1) * m_particle.MoveCorrection.x,(rand() % m_particle.MoveYNum + m_particle.MinMove.y + 1) * m_particle.MoveCorrection.y };
				// サイズ
				float         _size = (rand() % m_particle.SizeNum + m_particle.MinSize + 1) * m_particle.SizeCorrection;
				// サイズ変化量
				float         _sizeChange = m_particle.SizeChange;
				// セット
				_particle->SetParam(m_pos, _move, _size, _sizeChange);
				
				// リスト格納
				m_particle.ParticleList.push_back(_particle);
			}

			// クリック確認フラグON
			m_ClickFlg = true;
		}
	}
	// 暗転フラグがOFFだったら
	else if(!SceneManager::Instance().GetBlackAlphaFlg())
	{
		// クリック確認フラグOFF
		m_ClickFlg = false;
	}

	// パーティクル更新
	for (auto& _particle : m_particle.ParticleList)_particle->Update();
}

void Cursor::PostUpdate()
{
	// パーティクル削除
	ParticleDelete();
}

void Cursor::DrawSprite()
{
	// パーティクル描画
	for (auto& _particle : m_particle.ParticleList)_particle->DrawSprite();
	
	UIBase::DrawSprite();
}

void Cursor::Init()
{
	m_pivot = { 0.0f,1.0f };
	m_rect = { 0,0,long(62),long(97) };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Cursor/Cursors.png");
	srand(timeGetTime());
}

void Cursor::ParticleDelete()
{
	auto _particle = m_particle.ParticleList.begin();

	while (_particle != m_particle.ParticleList.end())
	{
		if ((*_particle)->IsExpired()) // 消滅フラグがONだったら
		{
			// 削除
			_particle = m_particle.ParticleList.erase(_particle);
		}
		else
		{
			_particle++;
		}
	}
}
