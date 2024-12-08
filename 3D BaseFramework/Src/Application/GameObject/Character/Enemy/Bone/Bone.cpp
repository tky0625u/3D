#include "Bone.h"
#include"../EnemyBase.h"
#include"../../Player/Player.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../Weapon/Sword/Sword.h"

void Bone::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");

	std::shared_ptr<Appeal> appeal = std::make_shared<Appeal>();
	m_state = appeal;
	m_actionType = EnemyBase::Action::AppealType;
	m_flow = EnemyBase::Flow::UpdateType;

	m_AppealEffectSize = 3.0f;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}


// Attack==========================================================================================
void Bone::Attack::Enter(std::shared_ptr<EnemyBase> owner)
{
	owner->SetFlow(EnemyBase::Flow::UpdateType);
}

void Bone::Attack::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack"))
	{
		owner->SetAnime("Attack", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		owner->IdolChange();
		return;
	}

	if (m_ActionFPS == 35)
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
	}
	if (50 <= m_ActionFPS && 67 >= m_ActionFPS)
	{
		HitCheck(owner);
		MoveForward(owner);
	}
	m_ActionFPS++;
}

void Bone::Attack::Exit(std::shared_ptr<EnemyBase> owner)
{

}

void Bone::Attack::HitCheck(std::shared_ptr<EnemyBase> owner)
{
	if (SceneManager::Instance().GetPlayer()->IsExpired() ||
		SceneManager::Instance().GetPlayer()->GetParam().Hp <= 0 ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::HitType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::GuardReactionType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::ParryType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::CounterType ||
		SceneManager::Instance().GetPlayer()->GetActionType() == Player::Action::RollType)return;

	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;
	if (owner->GetSword().expired() == false)
	{
		sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelTop().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center =owner->GetSword().lock()->GetModelCenter().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = owner->GetSword().lock()->GetModelBottom().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	else
	{
		sphereInfo.m_sphere.Center = owner->GetSwordMat().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	sphereInfo.m_sphere.Radius = 1.5f;
	sphereInfo.m_type = KdCollider::TypeDamage;
	std::list<KdCollider::CollisionResult> _List;

	//owner->GetDebugWire()->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 1,0,0,1 });

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &_List);
	}

	if (_List.size() != 0)
	{
		for (auto& ret : _List)
		{
			if (SceneManager::Instance().GetPlayer()->GetActionType() != Player::Action::GuardType)
			{
				KdEffekseerManager::GetInstance().Play("Player/hit_effe.efkefc", ret.m_hitPos, 1.0f, 0.8f, false);
			}
			SceneManager::Instance().GetPlayer()->Damage(owner->GetParam().Atk, owner->shared_from_this());
		}
	}
}
void Bone::Attack::MoveForward(std::shared_ptr<EnemyBase> owner)
{
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->GetAngle().y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(owner->GetForward(), nowRot);
	nowVec.Normalize();

	owner->SetMove(nowVec);
}
//=================================================================================================


// Stumble=========================================================================================
void Bone::Stumble::Enter(std::shared_ptr<EnemyBase> owner)
{
}

void Bone::Stumble::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Stumble"))
	{
		owner->SetAnime("Stumble", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		// Idol
		std::shared_ptr<Idol> _idol = std::make_shared<Idol>();
		owner->SetNextAction(_idol, EnemyBase::Action::IdolType);
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}

	StumbleAction(owner);
}

void Bone::Stumble::Exit(std::shared_ptr<EnemyBase> owner)
{

}

void Bone::Stumble::StumbleAction(std::shared_ptr<EnemyBase> owner)
{
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(owner->GetAngle().y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(owner->GetForward(), _nowRot);
	_nowVec.y = 0.0f;
	_nowVec *= -1.0f;
	_nowVec.Normalize();
	owner->SetMove(_nowVec, 0.1f);
}
//=================================================================================================
