#include "BoneAlpha.h"

// シーンマネジャ
#include"../../../../Scene/SceneManager.h"
// オブジェクトマネジャ
#include"../../../ObjectManager.h"
// プレイヤー
#include"../../Player/Player.h"


void BoneAlpha::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/BoneAlpha/BoneAlpha.gltf");

	std::shared_ptr<Appeal> appeal = std::make_shared<Appeal>();
	m_state = appeal;
	m_actionType = EnemyBase::Action::AppealType;
	m_flow = EnemyBase::Flow::UpdateType;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}

const Math::Vector3& BoneAlpha::GetFrontDir()
{
	// 現在の方向
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(m_forward, _nowRot);
	return _nowVec;
}


// Attack==========================================================================================
void BoneAlpha::Attack::Enter(std::shared_ptr<EnemyBase> owner)
{

}

void BoneAlpha::Attack::Update(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Attack"))
	{
		owner->SetAnime("Attack", false, 1.0f);
		return;
	}

	// アニメーションが終了したら待機状態へ
	if (owner->GetIsAnimator())
	{
		owner->IdolChange();
		return;
	}

	if (m_ActionFPS == 35)
	{
		// 攻撃前エフェクト
		KdEffekseerManager::GetInstance().Play("Enemy/AttackSignal/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
	}
	else 
	{
		// プレイヤーの方向に回転
		Math::Vector3 _playerPos = owner->GetTarget().lock()->GetPos(); // プレイヤー座標
		Math::Vector3 _dir = _playerPos - owner->GetPos(); // プレイヤーの方向
		_dir.y = 0.0f; // Y軸は考慮しない
		_dir.Normalize(); // 正規化
		owner->Rotate(_dir); // 回転
	}
	if (50 <= m_ActionFPS && 67 >= m_ActionFPS)
	{
		HitCheck(owner); // 攻撃判定
		MoveForward(owner); // 攻撃移動
	}
	if (58 == m_ActionFPS)
	{
		owner->GetObjManager().lock()->SetBoneAlphaBulletParam(owner->GetID()); // 弾生成
	}

	// FPS加算
	m_ActionFPS++;
}

void BoneAlpha::Attack::Exit(std::shared_ptr<EnemyBase> owner)
{

}
//=================================================================================================