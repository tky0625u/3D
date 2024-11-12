#pragma once

class EnemyBase;

class StageManager :public std::enable_shared_from_this<StageManager>
{
public:
	StageManager() {};
	~StageManager() {};

	void Load();
	void NextStage();
	void WaveCheck();
	bool IsWaveMax() { return m_IsWaveMax; }
	
	void SetMaxWave(int _wave) { m_MaxWave = _wave; }
	void SetMaxStage(int _stage) { m_MaxStage = _stage; }

	const int& GetnowWave()const  { return m_nowWave; }
	const int& GetMaxWave()const  { return m_MaxWave; }
	const int& GetnowStage()const { return m_nowStage; }
	const int& GetMaxStage()const { return m_MaxStage; }

private:
	int                          m_nowWave = 0;
	int                          m_MaxWave = 0;
	int                          m_nowStage = 1;
	int                          m_MaxStage = 0;
	bool                         m_IsWaveMax = false;
};