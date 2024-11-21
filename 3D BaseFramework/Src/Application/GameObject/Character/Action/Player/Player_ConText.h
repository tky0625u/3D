//#pragma once
//#include"Player_ActionState.h"
//#include"../../../Camera/CameraBase.h"
//
//class CameraBase;
//class EnemyBase;
//class BulletBase;
//
//class Player_ActionConText:public std::enable_shared_from_this<Player_ActionConText>
//{
//public:
//	Player_ActionConText(std::shared_ptr<Player_ActionState> initialState):m_state(initialState){}
//	~Player_ActionConText() {};
//
//	enum KeyType
//	{
//		MoveKey   = 1 << 0,
//		AttackKey = 1 << 1,
//		GuardKey  = 1 << 2,
//		RollKey   = 1 << 3,
//		LockONKey = 1 << 4,
//	};
//
//	void SetState(std::shared_ptr<Player_ActionState> _state) { m_state = _state; }
//	void SetBeforeKeyType(UINT _KeyType) { m_BeforeKeyType = _KeyType; }
//	void SetLockONFlg(bool _lockONFlg) { m_lockONFlg = _lockONFlg; }
//	void SetLockONTarget(std::shared_ptr<EnemyBase> _lockONTarget) { m_lockONTarget = _lockONTarget; }
//
//	std::shared_ptr<Player_ActionState> GetState()const { return m_state; }
//	UINT GetBeforeKeyType()const { return m_BeforeKeyType; }
//	const std::weak_ptr<EnemyBase> GetLockONTarget()const { return m_lockONTarget; }
//	const bool GetLockONFlg()const { return m_lockONFlg; }
//
//	void Idol()                                                            { m_state->Idol(); }               // 待ち
//	void Run()                                                             { m_state->Run(); }                // 走り
//	void Attack()                                                          { m_state->Attack(); }             // 攻撃
//	void Guard()                                                           { m_state->Guard(); }              // ガード
//	void GuardReaction()                                                   { m_state->GuardReaction(); }      // ガード時の反応
//	void Parry(std::shared_ptr<EnemyBase> _enemy)                          { m_state->Parry(_enemy); }        // 直接攻撃のパリィ
//	void Parry(std::shared_ptr<BulletBase> _bullet)                        { m_state->Parry(_bullet); }          // 遠距離攻撃のパリィ
//	void Counter()                                                         { m_state->Counter(); }            // 追撃
//	void Roll()                                                            { m_state->Roll(); }               // 回避
//	void Hit(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)   { m_state->Hit(_damage, _enemy); } // 直接攻撃による被弾
//	void Hit(int _damage = 0, std::shared_ptr<BulletBase> _bullet=nullptr) { m_state->Hit(_damage, _bullet); }   // 遠距離攻撃による被弾
//	void Crushing()                                                        { m_state->Crushing(); }           // 撃破
//	void Teleport()                                                        { m_state->Teleport(); }           // テレポート
//
//private:
//	std::shared_ptr<Player_ActionState> m_state;
//	UINT m_BeforeKeyType = 0;
//	std::weak_ptr<EnemyBase>m_lockONTarget;
//	bool                    m_lockONFlg = false;
//};