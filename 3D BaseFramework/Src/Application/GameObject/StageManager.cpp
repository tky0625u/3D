#include "StageManager.h"

// シーンマネジャ
#include"../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"ObjectManager.h"

// 次のステージ生成
void StageManager::NextStage()
{
	// 最大ウェーブフラグをOFF
	m_IsWaveMax = false;
	// ステージ数を増やす
	m_nowStage++;
	// 最大ウェーブ数を0
	m_MaxWave = 0;
	// 現在のウェーブを0
	m_nowWave = 1;
	// ステージ生成
	m_ObjectManager.lock()->CreateStage(shared_from_this());
	// 画面を明るく
	SceneManager::Instance().BlackAlphaChange(0.01f, false);
}

// ウェーブ管理
void StageManager::WaveCheck()
{
	// 現在のウェーブが最大&最大ウェーブフラグがOFFだったら
	if (m_MaxWave == m_nowWave && !m_IsWaveMax)
	{
		// 最大ウェーブフラグをON
		m_IsWaveMax = true;
		
		// 最終ステージだったらゲームクリア
		if (m_nowStage == m_MaxStage) { m_ObjectManager.lock()->GameClear(); }
		// 違う場合 ワープ解放
		else { m_ObjectManager.lock()->NextStageLiberation(); }
	}
	// 最大ウェーブフラグがOFFだったら
	else if (!m_IsWaveMax)
	{
		// ウェーブ数を増やす
		m_nowWave++;

		// 敵生成
		std::string _filePath = ("Asset/Json/Game/Enemy/Stage") + (std::to_string(m_nowStage)) + (".json"); // Json
		m_ObjectManager.lock()->SetEnemyParam(_filePath, shared_from_this());
	}
}