#include "BoneAlpha.h"
#include"../../../../Scene/SceneManager.h"
#include"../../Player/Player.h"
#include"../../../Weapon/Sword/Sword.h"

void BoneAlpha::Init()
{
	EnemyBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/BoneAlpha/BoneAlpha.gltf");

	std::shared_ptr<Appeal> appeal = std::make_shared<Appeal>();
	m_state = appeal;
	m_actionType = EnemyBase::Action::AppealType;
	m_flow = EnemyBase::Flow::UpdateType;

	m_AppealEffectSize = 3.0f;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_model, KdCollider::TypeDamage | KdCollider::TypeBump | KdCollider::TypeSight);
}

void BoneAlpha::Attack::Enter(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("IdolTOAttack"))
	{
		owner->SetAnime("IdolTOAttack", false, 0.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
		owner->SetFlow(EnemyBase::Flow::UpdateType);
		return;
	}

	Math::Vector3 _playerPos = owner->GetTarget().lock()->GetPos();
	Math::Vector3 _dir = _playerPos - owner->GetPos();
	_dir.y = 0.0f;
	_dir.Normalize();
	owner->Rotate(_dir);
}

void BoneAlpha::Attack::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack"))
	{
		owner->SetAnime("Attack", false, 1.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->SetFlow(EnemyBase::Flow::ExitType);
		return;
	}

	HitCheck(owner);
	MoveForward(owner);
}

void BoneAlpha::Attack::Exit(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("AttackToIdol"))
	{
		owner->SetAnime("AttackToIdol", false, 0.5f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->IdolChange();
		return;
	}
}