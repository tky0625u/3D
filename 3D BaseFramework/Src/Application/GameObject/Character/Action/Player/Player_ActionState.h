#pragma once
#include"../ActionBase.h"

class Player_ActionConText;
class Player;
class EnemyBase;

class Player_ActionState:public ActionBase
{
public:
	Player_ActionState() {};
	~Player_ActionState()override {};

	void AttackDamage();

	void Update()override;
	virtual void KeyCheck();
	virtual void ChangeAction() {};

	void SetActionType(UINT _ActionType) { m_ActionType = _ActionType; }
	void SetTarget(std::shared_ptr<Player> target) { m_target = target; }

	virtual void Idol(std::shared_ptr<Player_ActionConText> context);  //待ち
	virtual void Run(std::shared_ptr<Player_ActionConText> context);  //走り
	virtual void Attack(std::shared_ptr<Player_ActionConText> context);  //攻撃
	virtual void Guard(std::shared_ptr<Player_ActionConText> context);  //ガード
	virtual void GuardReaction(std::shared_ptr<Player_ActionConText> context);  //ガード時の反応
	virtual void Parry(std::shared_ptr<Player_ActionConText> context,std::shared_ptr<EnemyBase> _enemy);  //パリィ
	virtual void Counter(std::shared_ptr<Player_ActionConText> context);  //追撃
	virtual void Roll(std::shared_ptr<Player_ActionConText> context);  //回避
	virtual void Hit(std::shared_ptr<Player_ActionConText> context, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr);  //被弾

protected:
	std::weak_ptr<Player> m_target;
	UINT                  m_ActionType = 0;
};