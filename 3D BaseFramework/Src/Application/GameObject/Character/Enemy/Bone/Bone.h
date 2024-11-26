#pragma once
#include"../../Enemy/EnemyBase.h"

class Bone :public EnemyBase
{
public:
	Bone() {}
	~Bone()override {};

	virtual void Init()  override;

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

protected:

	class Attack :public StateBase
	{
	public:
		Attack() {};
		~Attack()override {};

		virtual void Enter (std::shared_ptr<EnemyBase> owner)override;
		virtual void Update(std::shared_ptr<EnemyBase> owner)override;
		virtual void Exit  (std::shared_ptr<EnemyBase> owner)override;

		void HitCheck   (std::shared_ptr<EnemyBase> owner);
		void MoveForward(std::shared_ptr<EnemyBase> owner);

	protected:
	};

	class Stumble :public StateBase
	{
	public:
		Stumble() {};
		~Stumble()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		void StumbleAction(std::shared_ptr<EnemyBase> owner);

	protected:

	};

};