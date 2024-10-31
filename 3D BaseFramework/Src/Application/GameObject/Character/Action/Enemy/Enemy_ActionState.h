#pragma once
#include"../ActionBase.h"

class Enemy_ConText;
class EnemyBase;

class Enemy_ActionState :public ActionBase
{
public:
	Enemy_ActionState() {};
	~Enemy_ActionState()override {};

	void AttackCheck();

	void SetTarget(std::shared_ptr<EnemyBase> target) { m_target = target; }

	virtual void Idol   () = 0;              //待ち
	virtual void Run    () = 0;              //走り
	virtual void Attack () = 0;              //攻撃
	virtual void Hit    (int _damage) = 0; //被弾
	virtual void Stumble() = 0;              //のけぞり

protected:
	std::weak_ptr<EnemyBase> m_target;
};