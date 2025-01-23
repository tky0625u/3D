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
	void AttackChange()override; // 攻撃
	//=========================================================================

	// ゲッター================================================================
	// 弾の発射位置
	const Math::Vector3& GetBulletPoint()const;
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