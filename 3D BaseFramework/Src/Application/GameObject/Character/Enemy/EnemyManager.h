#pragma once

class EnemyBase;

class EnemyManager:public KdGameObject
{
public:
	EnemyManager()							{};
	~EnemyManager()					override{};

	void DeleteEnemyList();
	void EnemyAttack();
	void EnemyRun();

	void AddEnemy(std::shared_ptr<EnemyBase> _enemy) { m_EnemyList.push_back(_enemy); }
	void SetEnemyAttack(std::shared_ptr<EnemyBase> _enemy) { m_EnemyAttackList.push_back(_enemy); }

	const std::vector<std::shared_ptr<EnemyBase>>& GetEnemyList()const { return m_EnemyList; }

	void PreUpdate()				override;
	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void PreDraw()					override;
	void DrawLit()					override;
	void DrawUnLit()				override;
	void DrawBright()				override;
	void DrawSprite()				override;

	void Init()                     override;

private:
	std::vector<std::shared_ptr<EnemyBase>> m_EnemyList;
	std::vector<std::weak_ptr<EnemyBase>>   m_EnemyAttackList;
};