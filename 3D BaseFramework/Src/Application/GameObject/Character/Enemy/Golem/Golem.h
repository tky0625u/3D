#pragma once
#include"../EnemyBase.h"

class Bullet;

class Golem :public EnemyBase,public std::enable_shared_from_this<Golem>
{
public:
	Golem() {};
	~Golem()override {};

	void Action() override;
	void Init()   override;

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
	void AttackChange()override;
	void Damage(int _damage)override
	{
		m_state->Damage(this, _damage);
	}

	const Math::Matrix GetBulletPoint()const { return m_model->FindWorkNode("BulletPoint")->m_worldTransform * m_mWorld; }
	const Math::Matrix GetQuakePoint() const { return m_model->FindWorkNode("QuakePoint")->m_worldTransform * m_mWorld; }

private:
	class StateBase
	{
	public:
		StateBase() {};
		virtual ~StateBase() {};

		void StateUpdate(Golem* owner)
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

		virtual void Enter (Golem* owner) {};
		virtual void Update(Golem* owner) {};
		virtual void Exit  (Golem* owner) {};

		virtual void Damage(Golem* owner, int _damage);

	};

	class Appeal :public StateBase
	{
	public:
		Appeal() {};
		~Appeal()override {};

		void Enter (Golem* owner)override;
		void Update(Golem* owner)override;
		void Exit  (Golem* owner)override;

		void Damage(Golem* owner, int _damage)override { return; }

	private:
		Effekseer::Handle m_handle = 0;
	};

	class Idol :public StateBase
	{
	public:
		Idol() {};
		~Idol()override {};

		void Enter (Golem* owner)override;
		void Update(Golem* owner)override;
		void Exit  (Golem* owner)override;

	private:

	};

	class Run :public StateBase
	{
	public:
		Run() {};
		~Run()override {};

		void Enter (Golem* owner)override;
		void Update(Golem* owner)override;
		void Exit  (Golem* owner)override;

		void Move(Golem* owner);

	private:

	};

	class Attack1 :public StateBase
	{
	public:
		Attack1() {};
		~Attack1()override {};

		void Enter (Golem* owner)override;
		void Update(Golem* owner)override;
		void Exit  (Golem* owner)override;

	private:
		std::weak_ptr<Bullet> m_bullet;
	};

	class Attack2 :public StateBase
	{
	public:
		Attack2() {};
		~Attack2()override {};

		void Enter (Golem* owner)override;
		void Update(Golem* owner)override;
		void Exit  (Golem* owner)override;

		void AttackHit(Golem* owner);

	private:
		Math::Vector3 m_playerPos = Math::Vector3::Zero;

	};

	class Attack3 :public StateBase
	{
	public:
		Attack3() {};
		~Attack3()override {};

		void Enter (Golem* owner)override;
		void Update(Golem* owner)override;
		void Exit  (Golem* owner)override;

		void AttackHit(Golem* owner);

	private:

	};

	class Crushing :public StateBase
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter (Golem* owner)override;
		void Update(Golem* owner)override;
		void Exit  (Golem* owner)override;

	private:

	};

	std::shared_ptr<StateBase> m_state = nullptr;
	std::shared_ptr<StateBase> m_NextState = nullptr;

	void SetNextAction(std::shared_ptr<StateBase> _action, UINT _actionType) {
		m_NextState = _action;
		m_actionType = _actionType;
	}
};