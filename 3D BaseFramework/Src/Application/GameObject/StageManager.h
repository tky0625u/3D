#pragma once

// オブジェクトマネジャ
class ObjectManager;

class StageManager :public std::enable_shared_from_this<StageManager>
{
public:
	StageManager() {};
	~StageManager() {};

	// 次のステージ生成
	void NextStage();
	// ウェーブ管理
	void WaveCheck();
	// 最大ウェーブフラグ
	bool IsWaveMax() { return m_IsWaveMax; }
	
	// セッター====================================================================================
	void SetObjectManager(std::shared_ptr<ObjectManager> _Obj) { m_ObjectManager = _Obj; } // オブジェクトマネジャ
	void SetMaxStage     (int _stage)                          { m_MaxStage = _stage; }    // 最大ステージ数
	void SetMaxWave      (int _wave)                           { m_MaxWave = _wave; }      // 最大ウェーブ数
	//=============================================================================================

	// ゲッター====================================================================================
	const int& GetnowWave()const  { return m_nowWave; }  // 現在のウェーブ数
	const int& GetMaxWave()const  { return m_MaxWave; }  // 最大ウェーブ数
	const int& GetnowStage()const { return m_nowStage; } // 現在のステージ数
	const int& GetMaxStage()const { return m_MaxStage; } // 最大ステージ数
	//=============================================================================================

private:
	std::weak_ptr<ObjectManager> m_ObjectManager;     // オブジェクトマネジャ
	int                          m_nowWave   = 0;     // 現在のウェーブ数
	int                          m_MaxWave   = 0;     // 最大ウェーブ数
	int                          m_nowStage  = 1;     // 現在のステージ数
	int                          m_MaxStage  = 0;     // 最大ステージ数
	bool                         m_IsWaveMax = false; // 最大ウェーブフラグ
};