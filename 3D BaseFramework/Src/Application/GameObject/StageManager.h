#pragma once

class ObjectManager;
class EnemyBase;

class StageManager :public std::enable_shared_from_this<StageManager>
{
public:
	StageManager() {};
	~StageManager() {};

	void Load();
	void Clear();
	void WaveCheck();
	
	void SetObjManager(std::shared_ptr<ObjectManager> _ObjManager) { m_ObjManager = _ObjManager; }
	void SetStageEnemyList(std::vector<std::weak_ptr<EnemyBase>> _enemyList) { m_StageEnemyList.push_back(_enemyList); }

	const int& GetnowWave()const  { return m_nowWave; }
	const int& GetnowStage()const { return m_nowStage; }
	const int& GetStageEnemyListNum()const { return m_StageEnemyList.size(); }

private:
	std::weak_ptr<ObjectManager> m_ObjManager;
	std::vector<std::weak_ptr<EnemyBase>>              m_nowStageEnemyList;
	std::vector<std::vector<std::weak_ptr<EnemyBase>>> m_StageEnemyList;
	int                          m_nowWave = 0;
	int                          m_nowStage = 0;
	int                          m_MaxStage = 0;
};