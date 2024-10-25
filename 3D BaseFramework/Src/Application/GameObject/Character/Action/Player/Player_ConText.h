#pragma once
#include"Player_ActionState.h"
#include"../../../Camera/CameraBase.h"

class CameraBase;
class EnemyBase;

class Player_ActionConText:public std::enable_shared_from_this<Player_ActionConText>
{
public:
	Player_ActionConText(std::shared_ptr<Player_ActionState> initialState):m_state(initialState){}
	~Player_ActionConText() {};

	enum ActionType
	{
		IdolType = 1 << 0,
		MoveType = 1 << 1,
		AttackType = 1 << 2,
		GuardType = 1 << 3,
		RollType = 1 << 4,
		LockONType = 1 << 5,
	};

	void SetState(std::shared_ptr<Player_ActionState> _state) { m_state = _state; }
	void SetBeforeActionType(UINT _ActionType) { m_BeforeActionType = _ActionType; }
	void SetLockONFlg(bool _lockONFlg) { m_lockONFlg = _lockONFlg; }
	void SetLockONTarget(std::shared_ptr<EnemyBase> _lockONTarget) { m_lockONTarget = _lockONTarget; }

	std::shared_ptr<Player_ActionState> GetState()const { return m_state; }
	UINT GetBeforeActionType()const { return m_BeforeActionType; }
	const std::weak_ptr<EnemyBase> GetLockONTarget()const { return m_lockONTarget; }
	const bool GetLockONFlg()const { return m_lockONFlg; }

	void Idol() { m_state->Idol(shared_from_this()); }                   //待ち
	void Run() { m_state->Run(shared_from_this()); }            //走り
	void Attack() {m_state->Attack(shared_from_this()); }                //攻撃
	void Guard() {m_state->Guard(shared_from_this()); }                  //ガード
	void GuardReaction() {m_state->GuardReaction(shared_from_this()); }  //ガード時の反応
	void Parry(std::shared_ptr<EnemyBase> _enemy) { m_state->Parry(shared_from_this(),_enemy); }//パリィ
	void Counter() { m_state->Counter(shared_from_this()); }             //追撃
	void Roll() { m_state->Roll(shared_from_this()); }          //回避
	void Hit(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr) { m_state->Hit(shared_from_this(), _damage, _enemy); } //被弾

private:
	std::shared_ptr<Player_ActionState> m_state;
	UINT m_BeforeActionType = 0;
	std::weak_ptr<EnemyBase>m_lockONTarget;
	bool                    m_lockONFlg = false;
};