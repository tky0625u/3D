#pragma once
#include"../EnemyBase.h"

class Bullet;

class Golem :public EnemyBase
{
public:
	Golem() {};
	~Golem()override {};

	void Action()    override;
	void PostUpdate()override;
	void Init()      override;

	void AttackChange()override;
	void HitChange()override { return; }

	const Math::Matrix GetBulletPoint()const { return m_model->FindWorkNode("BulletPoint")->m_worldTransform * m_mWorld; }
	const Math::Matrix GetQuakePoint() const { return m_model->FindWorkNode("QuakePoint")->m_worldTransform * m_mWorld; }

private:

	class Attack1 :public EnemyBase::StateBase
	{
	public:
		Attack1() {};
		~Attack1()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	private:
		std::weak_ptr<Bullet> m_bullet;
	};

	class Attack2 :public StateBase
	{
	public:
		Attack2() {};
		~Attack2()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		void AttackHit(std::shared_ptr<EnemyBase> owner);

	private:
		Math::Vector3 m_playerPos = Math::Vector3::Zero;

	};

	class Attack3 :public StateBase
	{
	public:
		Attack3() {};
		~Attack3()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		void AttackHit(std::shared_ptr<EnemyBase> owner);

	private:

	};

};