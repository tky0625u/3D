#pragma once
#include"../ActionBase.h"

class Enemy_ConText;
class EnemyBase;

class Enemy_ActionState :public ActionBase
{
public:
	Enemy_ActionState() {};
	~Enemy_ActionState()override {};

	bool AttackCheck();

	void SetTarget(std::shared_ptr<EnemyBase> target) { m_target = target; }

	virtual void Appeal (std::shared_ptr<Enemy_ConText> context) = 0;              //アピール
	virtual void Idol   (std::shared_ptr<Enemy_ConText> context) = 0;              //待ち
	virtual void Run    (std::shared_ptr<Enemy_ConText> context) = 0;              //走り
	virtual void Attack (std::shared_ptr<Enemy_ConText> context) = 0;              //攻撃
	virtual void Hit    (std::shared_ptr<Enemy_ConText> context, int _damage) = 0; //被弾
	virtual void Stumble(std::shared_ptr<Enemy_ConText> context) = 0;              //のけぞり

protected:
	std::weak_ptr<EnemyBase> m_target;
};