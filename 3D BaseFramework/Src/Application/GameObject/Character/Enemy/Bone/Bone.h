#pragma once
#include"../../Enemy/EnemyBase.h"

class Bone :public EnemyBase,public std::enable_shared_from_this<Bone>
{
public:
	Bone() {}
	~Bone()override {};

	void Action()override;
	void Init()  override;
	
	void IdolChange()  override
	{
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		m_NextState = _idol;
		m_NextActionType = Action::IdolType;
		m_flow = EnemyBase::Flow::UpdateType;
	}
	void RunChange()   override
	{
		std::shared_ptr<Run> _run = std::make_shared<Run>();
		m_NextState = _run;
		m_NextActionType = Action::RunType;
		m_flow = EnemyBase::Flow::EnterType;
	}
	void AttackChange()override
	{
		std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
		m_NextState = _attack;
		m_NextActionType = Action::AttackType;
		m_flow = EnemyBase::Flow::EnterType;
	}
	void StumbleChange()override
	{
		std::shared_ptr<Stumble> _stumble = std::make_shared<Stumble>();
		m_NextState = _stumble;
		m_NextActionType = Action::StumbleType;
		m_flow = EnemyBase::Flow::UpdateType;
	}
	void Damage(int _damage)override
	{
		m_state->Damage(this, _damage);
	}

private:
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		void StateUpdate(Bone* owner)
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

		virtual void Enter (Bone* owner) {};
		virtual void Update(Bone* owner) {};
		virtual void Exit  (Bone* owner) {};

		virtual void Damage(Bone* owner, int _damage);

	};

	class Appeal :public StateBase
	{
	public:
		Appeal() {};
		~Appeal()override {};

		void Enter (Bone* owner)override;
		void Update(Bone* owner)override;
		void Exit  (Bone* owner)override;

		void Damage(Bone* owner, int _damage)override { return; }

	private:
		Effekseer::Handle m_handle = 0;
	};

	class Idol :public StateBase
	{
	public:
		Idol() {};
		~Idol()override {};

		void Enter (Bone* owner)override;
		void Update(Bone* owner)override;
		void Exit  (Bone* owner)override;

	private:

	};

	class Run :public StateBase
	{
	public:
		Run() {};
		~Run()override {};

		void Enter (Bone* owner)override;
		void Update(Bone* owner)override;
		void Exit  (Bone* owner)override;

		void Move(Bone* owner);

	private:

	};

	class Attack :public StateBase
	{
	public:
		Attack() {};
		~Attack()override {};

		void Enter (Bone* owner)override;
		void Update(Bone* owner)override;
		void Exit  (Bone* owner)override;

		void HitCheck   (Bone* owner);
		void MoveForward(Bone* owner);

	private:
	};

	class Stumble :public StateBase
	{
	public:
		Stumble() {};
		~Stumble()override {};

		void Enter (Bone* owner)override;
		void Update(Bone* owner)override;
		void Exit  (Bone* owner)override;

		void StumbleAction(Bone* owner);

	private:

	};

	class Hit :public StateBase
	{
	public:
		Hit() {};
		~Hit()override {};

		void Enter (Bone* owner)override;
		void Update(Bone* owner)override;
		void Exit  (Bone* owner)override;

	private:

	};

	class Crushing :public StateBase
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter (Bone* owner)override;
		void Update(Bone* owner)override;
		void Exit  (Bone* owner)override;

	private:

	};

	std::shared_ptr<StateBase> m_state = nullptr;
	std::shared_ptr<StateBase> m_NextState = nullptr;

	void SetNextAction(std::shared_ptr<StateBase> _action, UINT _actionType) {
		m_NextState = _action;
		m_actionType = _actionType;
	}
};