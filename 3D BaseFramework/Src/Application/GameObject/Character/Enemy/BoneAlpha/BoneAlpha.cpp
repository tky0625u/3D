#include "BoneAlpha.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../ObjectManager.h"
#include"../../Player/Player.h"
#include"../../../Weapon/Sword/Sword.h"

#include"../../../../main.h"

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

const Math::Vector3& BoneAlpha::GetFrontDir()
{
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(m_forward, _nowRot);
	Application::Instance().m_log.AddLog("_nowVec:x=%.2f,y=%.2f,z=%.2f\n", _nowVec.x, _nowVec.y, _nowVec.z);
	return _nowVec;
}


// Attack==========================================================================================
void BoneAlpha::Attack::Enter(std::shared_ptr<EnemyBase> owner)
{
	owner->SetFlow(EnemyBase::Flow::UpdateType);
}

void BoneAlpha::Attack::Update(std::shared_ptr<EnemyBase> owner)
{
	if (!owner->IsAnimCheck("Attack"))
	{
		owner->SetAnime("Attack", false, 1.0f);
		return;
	}

	if (owner->GetIsAnimator())
	{
		owner->IdolChange();
		return;
	}

	if (m_ActionFPS == 35)
	{
		KdEffekseerManager::GetInstance().Play("Enemy/AttackSignal/BloodLance.efkefc", owner->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
	}
	else 
	{
		Math::Vector3 _playerPos = owner->GetTarget().lock()->GetPos();
		Math::Vector3 _dir = _playerPos - owner->GetPos();
		_dir.y = 0.0f;
		_dir.Normalize();
		owner->Rotate(_dir);
	}
	if (50 <= m_ActionFPS && 67 >= m_ActionFPS)
	{
		HitCheck(owner);
		MoveForward(owner);
	}
	if (58 == m_ActionFPS)
	{
		owner->GetObjManager().lock()->SetBoneAlphaBulletParam(owner->GetID());
	}

	m_ActionFPS++;
}

void BoneAlpha::Attack::Exit(std::shared_ptr<EnemyBase> owner)
{

}
//=================================================================================================