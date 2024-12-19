#pragma once

// 敵基底
class EnemyBase;

class EnemyManager:public KdGameObject
{
public:
	EnemyManager()							{};
	~EnemyManager()					override{};

	// 敵リスト削除
	void DeleteEnemyList();
	// 攻撃命令
	void EnemyAttack();
	// 移動命令
	void EnemyRun();

	// 敵リストに追加
	void AddEnemy(std::shared_ptr<EnemyBase> _enemy) { m_EnemyList.push_back(_enemy); }

	// 敵リスト
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
	void DrawDebug()                override;

	void Init()                     override;

private:
	// 敵リスト
	std::vector<std::shared_ptr<EnemyBase>> m_EnemyList;
	// 攻撃予定リスト
	std::vector<std::weak_ptr<EnemyBase>>   m_EnemyAttackList;
};