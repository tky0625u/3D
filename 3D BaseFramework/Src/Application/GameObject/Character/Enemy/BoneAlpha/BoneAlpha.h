#pragma once

// 骨 (基底クラス)
#include"../Bone/Bone.h"

class BoneAlpha :public Bone
{
public:
	BoneAlpha() {};
	~BoneAlpha()override {};

	void Init()  override;

	// 行動切り替え============================================================
	void AttackChange()override // 攻撃
	{
		std::shared_ptr<Attack> _attack = std::make_shared<Attack>();
		m_NextState = _attack;
		m_NextActionType = Action::AttackType;
		m_flow = EnemyBase::Flow::UpdateType;
	}
	//=========================================================================

	// ゲッター================================================================
	// 弾の発射位置
	const Math::Vector3& GetBulletPoint()const { return (m_model->FindWorkNode("BulletPoint")->m_worldTransform * m_mWorld).Translation(); }
	// 正面方向
	const Math::Vector3& GetFrontDir();
	//=========================================================================

private:

	// 攻撃
	class Attack :public Bone::Attack
	{
	public:
		Attack() {};
		~Attack()override {};

		void Enter (std::shared_ptr<EnemyBase> owner)override;
		void Update(std::shared_ptr<EnemyBase> owner)override;
		void Exit  (std::shared_ptr<EnemyBase> owner)override;

	private:
	};
};