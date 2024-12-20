#pragma once

// 敵基底
#include"../../Enemy/EnemyBase.h"

class Bone :public EnemyBase
{
public:
	Bone() {}
	~Bone()override {};

	virtual void Init()  override;

	// 行動切り替え============================================================
	void AttackChange()override // 攻撃
	{
		std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
		m_NextState = _attack;
		m_NextActionType = Action::AttackType;
		m_flow = EnemyBase::Flow::UpdateType;
	}
	void StumbleChange()override // よろけ
	{
		std::shared_ptr<Stumble> _stumble = std::make_shared<Stumble>();
		m_NextState = _stumble;
		m_NextActionType = Action::StumbleType;
		m_flow = EnemyBase::Flow::UpdateType;
	}
	//=========================================================================

protected:

	// 攻撃
	class Attack :public StateBase
	{
	public:
		Attack() {};
		~Attack()override {};

		virtual void Enter (std::shared_ptr<EnemyBase> owner)override;
		virtual void Update(std::shared_ptr<EnemyBase> owner)override;
		virtual void Exit  (std::shared_ptr<EnemyBase> owner)override;

		// 攻撃判定
		void HitCheck   (std::shared_ptr<EnemyBase> owner);
		// 攻撃移動
		void MoveForward(std::shared_ptr<EnemyBase> owner);

	protected:
	};

	// よろけ
	class Stumble :public StateBase
	{
	public:
		Stumble() {};
		~Stumble()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		// よろけ演出
		void StumbleAction(std::shared_ptr<EnemyBase> owner);

	protected:

	};

};