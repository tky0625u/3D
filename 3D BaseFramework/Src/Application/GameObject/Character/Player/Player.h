#pragma once

#include"../CharacterBase.h"

class ActionBase;
class Player_ActionConText;
class Player_ActionState;
class EnemyBase;
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

	void SetShield(std::shared_ptr<Shield> _shield) { m_shield = _shield; }
	void SetStaminaRecoveryTime(int _time) { m_StaminaRecoveryTime = _time; }
	void SetParryID(UINT _id) { m_ParryID = _id; }
	void SetStageManager(std::shared_ptr<StageManager> _stage) { m_StageManager = _stage; }
	void SetTeleportFlg(bool _teleport) { m_TeleportFlg = _teleport; }

	std::shared_ptr<Player_ActionConText>GetConText()const { return m_context; }
	int GetMaxStamina()const { return m_MaxStamina; }
	std::weak_ptr<Shield> GetShield() { return m_shield; }
	const int GetStaminaRecoveryTime()const { return m_StaminaRecoveryTime; }
	const UINT GetParryID()const { return m_ParryID; }
	const Math::Matrix& GetEnemyAttackPointMat() const { return (m_model->FindWorkNode("EnemyAttackPoint")->m_worldTransform) * m_mWorld; }
	const UINT& GetActionType()const { return m_actionType; }
	const std::weak_ptr<ObjectManager>& GetObjectManager()const { return m_ObjectManager; }
	const bool& GetTeleportFlg()const { return m_TeleportFlg; }

private:
	std::weak_ptr<StageManager>           m_StageManager;
	std::shared_ptr<Player_ActionConText> m_context        = nullptr;
	std::weak_ptr<Shield>                 m_shield;
	float                                 m_FocusBackRange = 2000.0f;
	int                                   m_MaxStamina     = 0;
	int                                   m_StaminaRecoveryTime = 0;
	bool                                  m_TeleportFlg    = false;
	UINT                                  m_ParryID = -1;

private:
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		virtual void Enter (Player* owner) {};
		virtual void Update(Player* owner) {};
		virtual void Exit  (Player* owner) {};

		virtual void ChangeState(Player* owner) = 0;
	};

	class Idol :public StateBase
	{
	public:
		Idol() {};
		~Idol()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Run :public StateBase
	{
	public:
		Run() {};
		~Run()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Attack :public StateBase
	{
	public:
		Attack() {};
		~Attack()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Counter :public StateBase
	{
	public:
		Counter() {};
		~Counter()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Roll :public StateBase
	{
	public:
		Roll() {};
		~Roll()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Guard :public StateBase
	{
	public:
		Guard() {};
		~Guard()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class GuardReaction :public StateBase
	{
	public:
		GuardReaction() {};
		~GuardReaction()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Parry :public StateBase
	{
	public:
		Parry() {};
		~Parry()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Hit :public StateBase
	{
	public:
		Hit() {};
		~Hit()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Crushing :public StateBase
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

	};

	class Teleport :public StateBase
	{
	public:
		Teleport() {};
		~Teleport()override {};

		void Enter (Player* owner)override;
		void Update(Player* owner)override;
		void Exit  (Player* owner)override;

		void ChangeState(Player* owner)override;

	private:

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