#pragma once

// ゲームカメラ
class GameCamera;
// プレイヤー
class Player;
// 魔法陣
class MagicPolygon;

class StageManager :public std::enable_shared_from_this<StageManager>
{
public:
	StageManager() {};
	~StageManager() {};

	// 次のステージ生成
	void NextStage();
	// ウェーブ管理
	bool WaveCheck();
	// ワープ解放時の演出の準備
	void NextStageLiberation();
	// ゲームクリア
	void GameClear();

	// ステージ数を動的に変更
	void DebugStage();

	// セッター====================================================================================
	void SetMaxStage();                                         // 最大ステージ数
	void SetMaxWave(int _wave);                                 // 最大ウェーブ数
	void SetCamera(std::shared_ptr<GameCamera> _camera);        // ゲームカメラ
	void SetPlayer(std::shared_ptr<Player> _player);            // プレイヤー
	void SetMagicPolygon(std::shared_ptr<MagicPolygon> _magic); // 魔法陣
	//=============================================================================================

	// ゲッター====================================================================================
	const int& GetnowWave()const;  // 現在のウェーブ数
	const int& GetMaxWave()const;  // 最大ウェーブ数
	const int& GetnowStage()const; // 現在のステージ数
	const int& GetMaxStage()const; // 最大ステージ数
	const bool IsWaveMax()const;   // 最大ウェーブフラグ
	//=============================================================================================

private:
	std::weak_ptr<GameCamera>    m_camera;            // ゲームカメラ
	std::weak_ptr<Player>        m_player;            // プレイヤー
	std::weak_ptr<MagicPolygon>  m_magic;             // 魔法陣
	int                          m_nowWave   = 1;     // 現在のウェーブ数
	int                          m_MaxWave   = 0;     // 最大ウェーブ数
	int                          m_nowStage  = 1;     // 現在のステージ数
	int                          m_MaxStage  = 0;     // 最大ステージ数
	bool                         m_IsWaveMax = false; // 最大ウェーブフラグ
};