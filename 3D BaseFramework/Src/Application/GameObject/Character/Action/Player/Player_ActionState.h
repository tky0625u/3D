#pragma once
#include"../ActionBase.h"

class Player;
class EnemyBase;

class Player_ActionState:public ActionBase
{
public:
	Player_ActionState() {};
	~Player_ActionState()override {};

	void AttackDamage();
	void LockON();

	void Update()override;
	virtual void KeyCheck();
	virtual void ChangeAction() {};

	void SetKeyType(UINT _KeyType) { m_KeyType = _KeyType; }
	void SetTarget(std::shared_ptr<Player> target) { m_target = target; }

	virtual void Idol();                                                             // 待ち
	virtual void Run();                                                              // 走り
	virtual void Attack();                                                           // 攻撃
	virtual void Guard();                                                            // ガード
	virtual void GuardReaction();                                                    // ガード時の反応
	virtual void Parry(std::shared_ptr<EnemyBase> _enemy);                           // パリィ
	virtual void Counter();                                                          // 追撃
	virtual void Roll();                                                             // 回避
	virtual void Hit(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr);  // 被弾
	virtual void Crushing();                                                         // 撃破

protected:
	std::weak_ptr<Player> m_target;
	UINT                  m_KeyType = 0;
};