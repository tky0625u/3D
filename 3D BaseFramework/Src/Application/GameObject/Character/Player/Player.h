#pragma once

#include"../CharacterBase.h"

class ActionBase;
class Player_ActionConText;
class Player_ActionState;
class EnemyBase;
class BulletBase;
class Shield;
class StageManager;
class ObjectManager;
class StageManager;

class Player :public CharacterBase, public std::enable_shared_from_this<Player>
{
public:
	enum Action
	{
		IdolType,
		AttackType,
		RunType,
		RollType,
		GuardType,
		GuardReactionType,
		ParryType,
		CounterType,
		HitType,
		CrushingType,
		TeleportType
	};

	Player()                  {}
	~Player()        override {};

	void Update()    override;
	void Action()    override;
	void PostUpdate()override;
	void Init()      override;

	void CrushingAction()override;
	void LockONCheck();
	void NextStageCheck();
	void StaminaRecovery() { 
		if (m_StaminaRecoveryTime > 0)return;
		m_param.Sm++;
		if (m_param.Sm >= m_MaxStamina)m_param.Sm = m_MaxStamina;
	}

	void IdolChange();
	void TeleportChange();
	void Damage(int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr) { m_state->Damage(shared_from_this(), _damage, _enemy); }  // 直接攻撃による被弾
	void Damage(int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr) { m_state->Damage(shared_from_this(), _damage, _bullet); } // 遠距離攻撃による被弾

	void SetShield(std::shared_ptr<Shield> _shield) { m_shield = _shield; }
	void SetParryID(UINT _id) { m_ParryID = _id; }
	void SetStageManager(std::shared_ptr<StageManager> _stage) { m_StageManager = _stage; }
	void SetTeleportFlg(bool _teleport) { m_TeleportFlg = _teleport; }

	int GetMaxStamina()const { return m_MaxStamina; }
	std::weak_ptr<Shield> GetShield() { return m_shield; }
	const std::weak_ptr<EnemyBase>& GetLockONTarget()const { return m_LockONTarget; }
	const int GetStaminaRecoveryTime()const { return m_StaminaRecoveryTime; }
	const UINT GetParryID()const { return m_ParryID; }
	const Math::Matrix& GetEnemyAttackPointMat() const { return (m_model->FindWorkNode("EnemyAttackPoint")->m_worldTransform) * m_mWorld; }
	const Math::Matrix& GetCameraPointMat()const { return (m_model->FindWorkNode("CameraPoint")->m_worldTransform) * m_mWorld; }
	const UINT& GetActionType()const { return m_actionType; }
	const std::weak_ptr<ObjectManager>& GetObjectManager()const { return m_ObjectManager; }
	const bool& GetLockONFlg()const { return m_LockONFlg; }
	const bool& GetTeleportFlg()const { return m_TeleportFlg; }

private:
	std::weak_ptr<StageManager>  m_StageManager;
	std::weak_ptr<Shield>        m_shield;
	std::weak_ptr<EnemyBase>     m_LockONTarget;
	float                        m_FocusBackRange = 2000.0f;
	int                          m_MaxStamina     = 0;
	int                          m_StaminaRecoveryTime = 0;
	bool                         m_TeleportFlg    = false;
	bool                         m_LockONFlg      = false;
	UINT                         m_ParryID        = -1;

private:
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		void StateUpdate(std::shared_ptr<Player> owner);

		virtual void Enter (std::shared_ptr<Player> owner) {};
		virtual void Update(std::shared_ptr<Player> owner) {};
		virtual void Exit  (std::shared_ptr<Player> owner) {};

		virtual void Event(std::shared_ptr<Player> owner)  {};
		virtual void ChangeState(std::shared_ptr<Player> owner) = 0;

		virtual void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr);   // 直接攻撃による被弾
		virtual void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr); // 遠距離攻撃による被弾

		void AttackHit(std::shared_ptr<Player> owner);

	protected:
		float m_ActionFPS = 0.0f;
	};

	class Idol :public StateBase
	{
	public:
		Idol() {};
		~Idol()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	class Run :public StateBase
	{
	public:
		Run() {};
		~Run()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void Event (std::shared_ptr<Player> owner)override;
		void ChangeState(std::shared_ptr<Player> owner)override;

	private:

	};

	class Attack :public StateBase
	{
	public:
		Attack() {};
		~Attack()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void Event (std::shared_ptr<Player> owner)override;
		void Attack1(std::shared_ptr<Player> owner);
		void Attack2(std::shared_ptr<Player> owner);
		void Attack3(std::shared_ptr<Player> owner);
		void AttackDirCheck(std::shared_ptr<Player> owner);
		void ChangeState(std::shared_ptr<Player> owner)override;

	private:
		Math::Vector3    m_AttackDir  = Math::Vector3::Zero;
		static const int AttackNUM    = 3;
		int              m_atkNum     = 1;
		int              m_ChangeTime = 0;
	};

	class Counter :public StateBase
	{
	public:
		Counter() {};
		~Counter()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override { return; } // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override { return; } // 遠距離攻撃による被弾

	private:

	};

	class Roll :public StateBase
	{
	public:
		Roll() {};
		~Roll()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void Event (std::shared_ptr<Player> owner)override;
		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override { return; } // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override { return; } // 遠距離攻撃による被弾

	private:

	};

	class Guard :public StateBase
	{
	public:
		Guard() {};
		~Guard()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void Event (std::shared_ptr<Player> owner)override;
		void ChangeState(std::shared_ptr<Player> owner)override;

		void GuardRotate(std::shared_ptr<Player> owner, Math::Vector3 _dir);
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override; // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override; // 遠距離攻撃による被弾

		void SetGuardTime(int _time) { m_guardTime = _time; }

	private:
		int m_guardTime = 0;
	};

	class GuardReaction :public StateBase
	{
	public:
		GuardReaction() {};
		~GuardReaction()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override { return; } // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override { return; } // 遠距離攻撃による被弾

	private:

	};

	class Parry :public StateBase
	{
	public:
		Parry() {};
		~Parry()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override { return; } // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override { return; } // 遠距離攻撃による被弾

	private:

	};

	class Hit :public StateBase
	{
	public:
		Hit() {};
		~Hit()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override { return; } // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override { return; } // 遠距離攻撃による被弾

	private:

	};

	class Crushing :public StateBase
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override { return; } // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override { return; } // 遠距離攻撃による被弾

	private:

	};

	class Teleport :public StateBase
	{
	public:
		Teleport() {};
		~Teleport()override {};

		void Enter (std::shared_ptr<Player> owner)override;
		void Update(std::shared_ptr<Player> owner)override;
		void Exit  (std::shared_ptr<Player> owner)override;

		void ChangeState(std::shared_ptr<Player> owner)override;

		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<EnemyBase> _enemy = nullptr)  override { return; } // 直接攻撃による被弾
		void Damage(std::shared_ptr<Player> owner, int _damage = 0, std::shared_ptr<BulletBase> _bullet = nullptr)override { return; } // 遠距離攻撃による被弾

	private:
		Effekseer::Handle m_handle = 0;
	};

	enum KeyType
	{
		MoveKey   = 1 << 0,
		AttackKey = 1 << 1,
		GuardKey  = 1 << 2,
		RollKey   = 1 << 3,
		LockONKey = 1 << 4,
	};

	std::shared_ptr<StateBase> m_state          = nullptr;
	std::shared_ptr<StateBase> m_NextState      = nullptr;
	UINT                       m_flow           = CharacterBase::Flow::UpdateType;
	UINT                       m_actionType     = Action::IdolType;
	UINT                       m_NextActionType = m_actionType;
	UINT                       m_keyType        = 0;
	UINT                       m_BeforeKeyType  = 0;

	void SetNextState(std::shared_ptr<StateBase> _next, UINT _action)
	{
		m_NextState = _next;
		m_NextActionType = _action;
	}

	void KeyCheck();
};