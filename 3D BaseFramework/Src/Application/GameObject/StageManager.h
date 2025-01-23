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
	
	// セッター====================================================================================
	void SetObjectManager(std::shared_ptr<ObjectManager> _Obj); // オブジェクトマネジャ
	void SetMaxStage(int _stage);                               // 最大ステージ数
	void SetMaxWave(int _wave);                                 // 最大ウェーブ数
	//=============================================================================================

	// ゲッター====================================================================================
	const int& GetnowWave()const;  // 現在のウェーブ数
	const int& GetMaxWave()const;  // 最大ウェーブ数
	const int& GetnowStage()const; // 現在のステージ数
	const int& GetMaxStage()const; // 最大ステージ数
	const bool IsWaveMax()const;   // 最大ウェーブフラグ
	//=============================================================================================

private:
	std::weak_ptr<ObjectManager> m_ObjectManager;     // オブジェクトマネジャ
	int                          m_nowWave   = 0;     // 現在のウェーブ数
	int                          m_MaxWave   = 0;     // 最大ウェーブ数
	int                          m_nowStage  = 1;     // 現在のステージ数
	int                          m_MaxStage  = 0;     // 最大ステージ数
	bool                         m_IsWaveMax = false; // 最大ウェーブフラグ
};