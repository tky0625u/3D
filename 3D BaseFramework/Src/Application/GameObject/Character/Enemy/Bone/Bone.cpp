#include "Bone.h"

// シーンマネジャ
#include"../../../../Scene/SceneManager.h"
// 敵基底
#include"../EnemyBase.h"
// プレイヤー
#include"../../Player/Player.h"

void Bone::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");

	std::shared_ptr<Appeal> appeal = std::make_shared<Appeal>();
	m_state = appeal;
	m_actionType = EnemyBase::Action::AppealType;
	m_flow = EnemyBase::Flow::UpdateType;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}


// Attack==========================================================================================
void Bone::Attack::Enter(std::shared_ptr<EnemyBase> owner)
{

}

void Bone::Attack::Update(std::shared_ptr<EnemyBase> owner)
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
		// Idol
		owner->IdolChange();
		return;
	}

	if (m_ActionFPS == 35)
	{
		// 攻撃前エフェクト
		KdEffekseerManager::GetInstance().Play("Enemy/AttackSignal/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
	}
	if (50 <= m_ActionFPS && 67 >= m_ActionFPS)
	{
		// 攻撃判定
		HitCheck(owner);
		// 攻撃移動
		MoveForward(owner);
	}
	// FPS加算
	m_ActionFPS++;
}

void Bone::Attack::Exit(std::shared_ptr<EnemyBase> owner)
{

}

void Bone::Attack::HitCheck(std::shared_ptr<EnemyBase> owner)
{
	// プレイヤーが攻撃を受けない状態ならリターン
	if (SceneManager::Instance().GetPlayer()->IsExpired() ||                                          // 消滅フラグがON
		SceneManager::Instance().GetPlayer()->GetParam().Hp <= 0 ||                                   // HPがない
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::HitType ||           // 被弾状態
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::GuardReactionType || // 防御反応状態
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::ParryType ||         // パリィ状態
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::CounterType ||       // カウンター状態
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::RollType)return;     // 回避状態

	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	// 中心
	sphereInfo.m_sphere.Center = owner->GetSwordMat().Translation();
	// 半径
	sphereInfo.m_sphere.Radius = owner->GetAttackSphereSize();
	// 対象
	sphereInfo.m_type = KdCollider::TypeDamage;

	// デバッグ
	//owner->GetDebugWire()->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 1,0,0,1 });

	std::list<KdCollider::CollisionResult> _List; // 当たり判定リスト
	SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &_List); // 当たり判定

	if (_List.size() != 0)
	{
		for (auto& ret : _List)
		{
			if (SceneManager::Instance().GetPlayer()->GetActionType() != Player::Action::GuardType) // プレイヤーが防御していなかったらヒットエフェクトを出す
			{
				// ヒットエフェクト
				KdEffekseerManager::GetInstance().Play("Player/Hit/hit_effe.efkefc", ret.m_hitPos, 1.0f, 0.8f, false);
			}
			// ダメージ
			SceneManager::Instance().GetPlayer()->Damage(owner->GetParam().Atk, owner->shared_from_this());
		}
	}
}
void Bone::Attack::MoveForward(std::shared_ptr<EnemyBase> owner)
{
	// 現在の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->GetAngle().y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->GetForward(), nowRot);
	nowVec.Normalize(); // 正規化

	// 移動
	owner->SetMove(nowVec);
}
//=================================================================================================


// Stumble=========================================================================================
void Bone::Stumble::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void Bone::Stumble::Update(std::shared_ptr<EnemyBase> owner)
{
	// アニメーション変更
	if (!owner->IsAnimCheck("Stumble"))
	{
		owner->SetAnime("Stumble", false, 1.5f);
		return;
	}

	// アニメーションが終了したら待機状態へ
	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}

	// よろけ演出
	StumbleAction(owner);
}

void Bone::Stumble::Exit(std::shared_ptr<EnemyBase> owner)
{

}

void Bone::Stumble::StumbleAction(std::shared_ptr<EnemyBase> owner)
{
	// 現在の方向
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->GetAngle().y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(owner->GetForward(), _nowRot);
	_nowVec.y = 0.0f; // Y軸は考慮しない
	_nowVec *= -1.0f; // 現在の方向とは逆の方向
	_nowVec.Normalize(); // 正規化
	owner->SetMove(_nowVec, owner->GetStumbleMove()); // 移動
}
//=================================================================================================
