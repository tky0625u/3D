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
	void AttackChange()override;   // 攻撃
	void StumbleChange()override;  // よろけ
	void CrushingChange()override; // 消滅
	//=========================================================================

protected:

	// 出現演出
	class Appeal :public EnemyBase::Appeal
	{
	public:
		Appeal() {};
		~Appeal()override {};

		void Update(std::shared_ptr<EnemyBase> owner)override;

	private:

	};

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

	// 消滅
	class Crushing :public EnemyBase::Crushing
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter(std::shared_ptr<EnemyBase> owner)override;

	private:

	};

};