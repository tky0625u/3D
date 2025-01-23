#pragma once

// 敵基底 
#include"../EnemyBase.h"

// 弾
class Bullet;

class Golem :public EnemyBase
{
public:
	Golem() {};
	~Golem()override {};

	void Action()    override;
	void PostUpdate()override;
	void Init()      override;

	// 行動切り替え============================================================
	void AttackChange()override;            // 攻撃
	void HitChange()   override { return; } // 被弾
	void CrushingChange()override;          // 消滅
	//=========================================================================

	// 横方向当たり判定
	void BumpCheck()   override;

	// セッター================================================================
	void SetAttack1Angle(float _angle); // 弾攻撃の射程角度
	void SetAttack2Dist(float _dist);   // ジャンプ攻撃の最短距離
	//=========================================================================

	// ゲッター================================================================
	// 弾発射位置
	const Math::Vector3& GetBulletPoint() const;
	// 地震位置
	const Math::Vector3& GetQuakePoint()  const;
	// 弾攻撃の射程角度
	const float&         GetAttack1Angle()const;
	// ジャンプ攻撃の最短距離
	const float&         GetAttack2Dist() const;
	//=========================================================================

private:
	// 弾攻撃の射程角度
	float m_Attack1Angle = 0.0f;
	
	// ジャンプ攻撃の最短距離
	float m_Attack2Dist  = 0.0f;

	// 出現演出
	class Appeal :public EnemyBase::Appeal
	{
	public:
		Appeal() {};
		~Appeal()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	private:

	};

	// 弾攻撃
	class Attack1 :public EnemyBase::StateBase
	{
	public:
		Attack1() {};
		~Attack1()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	private:
		// 弾
		std::weak_ptr<Bullet> m_bullet;
	};

	// ジャンプ攻撃
	class Attack2 :public StateBase
	{
	public:
		Attack2() {};
		~Attack2()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		// 攻撃判定
		void AttackHit(std::shared_ptr<EnemyBase> owner);

	private:
		// プレイヤー座標
		Math::Vector3 m_playerPos = Math::Vector3::Zero;
		// 攻撃範囲
		float         m_AttackSphereRange = 0.0f;

	};

	// 叩きつけ攻撃
	class Attack3 :public StateBase
	{
	public:
		Attack3() {};
		~Attack3()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

		// 攻撃判定
		void AttackHit(std::shared_ptr<EnemyBase> owner);

	private:
		// 攻撃範囲
		float         m_AttackSphereRange = 0.0f;
	};

	// 消滅
	class Crushing :public EnemyBase::Crushing
	{
	public:
		Crushing() {};
		~Crushing()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	private:

	};
};