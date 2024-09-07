#pragma once
#include"../ActionBase.h"

class Enemy_ConText;
class Player;
class EnemyBase;

class Enemy_ActionState :public ActionBase
{
public:
	Enemy_ActionState() {};
	~Enemy_ActionState()override {};

	bool ChaseCheck();

	void SetConText(std::shared_ptr<Enemy_ConText> _conText) { if (m_conText == nullptr)m_conText = _conText; }
	void SetPlayer(std::shared_ptr<Player> _player) { m_player = _player; }
	void SetTarget(std::shared_ptr<EnemyBase> target) { m_target = target; }

	virtual void Idol   (std::shared_ptr<Enemy_ConText> context) { return; }  //待ち
	virtual void Run    (std::shared_ptr<Enemy_ConText> context) { return; }  //走り
	virtual void Attack (std::shared_ptr<Enemy_ConText> context) { return; }  //攻撃
	virtual void Hit    (std::shared_ptr<Enemy_ConText> context) { return; }  //被弾
	virtual void Stumble(std::shared_ptr<Enemy_ConText> context) { return; }  //のけぞり

protected:
	std::weak_ptr<EnemyBase> m_target;
	std::shared_ptr<Enemy_ConText> m_conText = nullptr;
	std::weak_ptr<Player> m_player;
};