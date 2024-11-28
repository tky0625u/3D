#pragma once
#include"../CharacterBase.h"

class Enemy_HP;
class Player;

class EnemyBase :public CharacterBase,public std::enable_shared_from_this<EnemyBase>
{
public:
	enum Flow
	{
		EnterType,
		UpdateType,
		ExitType
	};

	enum Action
	{
		IdolType,
		AttackType,
		RunType,
		StumbleType,
		HitType,
		AppealType,
		CrushingType
	};

	EnemyBase() {}
	~EnemyBase()override {};

	void PostUpdate()    override;
	void DrawOutLine();
	virtual void Action()override;
	virtual void Init()  override;

	void CrushingAction()override;
	void IdolChange()
	{
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		m_NextState = _idol;
		m_NextActionType = Action::IdolType;
		m_flow = EnemyBase::Flow::UpdateType;
	}
	void RunChange()
	{
		std::shared_ptr<Run> _run = std::make_shared<Run>();
		m_NextState = _run;
		m_NextActionType = Action::RunType;
		m_flow = EnemyBase::Flow::EnterType;
	}
	virtual void AttackChange(){};
	virtual void StumbleChange() {};
	void Damage(int _damage) { m_state->Damage(shared_from_this(), _damage); }
	virtual void HitChange()
	{
		std::shared_ptr<Hit> _hit = std::make_shared<Hit>();
		m_NextState = _hit;
		m_NextActionType = EnemyBase::Action::HitType;
		m_flow = EnemyBase::Flow::UpdateType;
	}

	void SetTarget(std::shared_ptr<Player> _target) { m_Target = _target; }
	void SetLockONFlg(bool _LockONFlg) { m_LockONFlg = _LockONFlg; }
	void SetFlow(UINT _flow) { m_flow = _flow; }

	const std::weak_ptr<Player>& GetTarget()const { return m_Target; }
	Math::Matrix GettoTargetRotateYMatrix(std::weak_ptr<Player> _target);
	Math::Matrix GetHPMat()const { return m_model->FindWorkNode("HP")->m_worldTransform * m_mWorld; }
	Math::Matrix GetAttackStartPointMat()const { return m_model->FindWorkNode("AttackStartPoint")->m_worldTransform * m_mWorld; }
	Math::Matrix GetLockPointMat()const { return m_model->FindWorkNode("LockONPoint")->m_worldTransform * m_mWorld; }
	UINT GetActionType()const { return m_actionType; }
	const bool& GetLockONFlg()const { return m_LockONFlg; }
	const float& GetAppealEffectSize()const { return m_AppealEffectSize; }

protected:
	std::weak_ptr<Player>              m_Target;
	std::weak_ptr<KdEffekseerObject>   m_Effect;
	std::weak_ptr<EnemyBase>           m_leaveEnemy;
	bool                               m_LockONFlg = false;

protected:
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		void StateUpdate(std::shared_ptr<EnemyBase> owner)
		{
			switch (owner->m_flow)
			{
			case EnemyBase::Flow::EnterType:
				Enter(owner);
				break;
			case EnemyBase::Flow::UpdateType:
				Update(owner);
				break;
			case EnemyBase::Flow::ExitType:
				Exit(owner);
				break;
			default:
				break;
			}
		}

		virtual void Enter (std::shared_ptr<EnemyBase> owner) {};
		virtual void Update(std::shared_ptr<EnemyBase> owner) {};
		virtual void Exit  (std::shared_ptr<EnemyBase> owner) {};

		virtual void Damage(std::shared_ptr<EnemyBase> owner, int _damage);

	protected:
		float m_ActionFPS = 0.0f;

	};

	class Appeal :public StateBase
	{
	public:
		Appeal() {};
		~Appeal()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		void Damage(std::shared_ptr<EnemyBase> owner, int _damage)override { return; }

	protected:
		Effekseer::Handle m_handle = 0;
	};

	class Idol :public StateBase
	{
	public:
		Idol() {};
		~Idol()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	protected:

	};

	class Run :public StateBase
	{
	public:
		Run() {};
		~Run()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		void Move(std::shared_ptr<EnemyBase> owner);
		bool PlayerDistCheck(std::shared_ptr<EnemyBase> owner, std::weak_ptr<EnemyBase> leave);
		void EnemyCheck(std::shared_ptr<EnemyBase> owner);
		void Leave(std::shared_ptr<EnemyBase> owner);

	protected:
		Math::Vector3 m_moveDir  = Math::Vector3::Zero;
		Math::Vector3 m_leaveDir = Math::Vector3::Zero;
	};

	class Hit :public StateBase
	{
	public:
		Hit() {};
		~Hit()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	protected:

	};

	class Crushing :public StateBase
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	protected:

	};

	std::shared_ptr<StateBase> m_state = nullptr;
	std::shared_ptr<StateBase> m_NextState = nullptr;
	UINT                       m_flow = CharacterBase::Flow::UpdateType;
	UINT                       m_actionType = Action::IdolType;
	UINT                       m_NextActionType = m_actionType;
	float                      m_AppealEffectSize = 1.0f;

public:
	void SetNextAction(std::shared_ptr<StateBase> _action, UINT _actionType) {
		m_NextState = _action;
		m_actionType = _actionType;
	}

	void SetAppealEffectSize(float _size) { m_AppealEffectSize = _size; }
};