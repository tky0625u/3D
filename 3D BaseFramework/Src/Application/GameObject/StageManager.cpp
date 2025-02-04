#include "StageManager.h"
#include"../tinygltf/json.hpp"

// シーンマネジャ
#include"../Scene/SceneManager.h"
// 魔法陣
#include"Stage/MagicPolygon/MagicPolygon.h"
// プレイヤー
#include"Character/Player/Player.h"
// ゲームカメラ
#include"Camera/GameCamera/GameCamera.h"

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
}

// ウェーブ管理
bool StageManager::WaveCheck()
{
	// 現在のウェーブが最大&最大ウェーブフラグがOFFだったら
	if (m_MaxWave == m_nowWave && !m_IsWaveMax)
	{
		// 最大ウェーブフラグをON
		m_IsWaveMax = true;
		return m_IsWaveMax;
	}
	// 最大ウェーブフラグがOFFだったら
	else if (!m_IsWaveMax)
	{
		// ウェーブ数を増やす
		m_nowWave++;
		return m_IsWaveMax;
	}

	return m_IsWaveMax;
}

void StageManager::NextStageLiberation()
{
	if (m_magic.lock()->GetTeleport())return;
	if (m_camera.lock()->GetCameraType() == GameCamera::CameraType::FixedType)return;

	// スロー切り替え
	if (SceneManager::Instance().GetSlowFlg())SceneManager::Instance().SlowChange();

	// テレポート開放演出
	m_magic.lock()->NextChange();

	// テレポート位置
	m_camera.lock()->FixedChange();

	// 待機
	m_player.lock()->IdolChange();
}

void StageManager::GameClear()
{
	// クリア位置
	m_camera.lock()->ClearChange();

	// 待機
	m_player.lock()->IdolChange();
}

void StageManager::DebugStage()
{
	static int _stageNum = 1;
	ImGui::Text((const char*)u8"ステージ数：%d", m_MaxStage);
	ImGui::SliderInt("StageNum", &_stageNum, 100, 1);
	if (ImGui::Button((const char*)u8"ステージ数変更"))
	{
		m_MaxStage = _stageNum;

		nlohmann::json _json;

		_json["Stage"]["StageNum"] = m_MaxStage;

		// ファイルに保存
		std::ofstream _file("Asset/Json/Game/Stage/Stage.json");
		if (_file.is_open())
		{
			_file << _json.dump();
			_file.close();
		}
	}
}

// セッター========================================================================================
// 最大ステージ数
void StageManager::SetMaxStage()
{
	//jsonファイル
	std::string fileName = ("Asset/Json/Game/Stage/Stage.json");

	std::ifstream ifs(fileName.c_str());
	nlohmann::json _json;
	if (ifs.is_open())
	{
		ifs >> _json;
	}

	for (auto& _stage : _json)
	{
		int _stageNum = 0;
		_stageNum = _stage["StageNum"];
		m_MaxStage = _stageNum;
	}

	ifs.close();
}

// 最大ウェーブ数
void StageManager::SetMaxWave(int _wave)
{
	m_MaxWave = _wave;
}

// ゲームカメラ
void StageManager::SetCamera(std::shared_ptr<GameCamera> _camera)
{
	m_camera = _camera;
}

// プレイヤー
void StageManager::SetPlayer(std::shared_ptr<Player> _player)
{
	m_player = _player;
}

// 魔法陣
void StageManager::SetMagicPolygon(std::shared_ptr<MagicPolygon> _magic)
{
	m_magic = _magic;
}
//=================================================================================================

// ゲッター========================================================================================
// 現在のウェーブ数
const int& StageManager::GetnowWave() const
{
	return m_nowWave;
}

// 最大ウェーブ数
const int& StageManager::GetMaxWave() const
{
	return m_MaxWave;
}

// 現在のステージ数
const int& StageManager::GetnowStage() const
{
	return m_nowStage;
}

// 最大ステージ数
const int& StageManager::GetMaxStage() const
{
	return m_MaxStage;
}

// 最大ウェーブフラグ
const bool StageManager::IsWaveMax() const
{
	return m_IsWaveMax;
}
//=================================================================================================