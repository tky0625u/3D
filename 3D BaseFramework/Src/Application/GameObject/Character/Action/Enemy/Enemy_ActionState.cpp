#include "Enemy_ActionState.h"
#include"../../../../Scene/SceneManager.h"
#include"../../Enemy/EnemyBase.h"
#include"../../Player/Player.h"
#include"Enemy_ConText.h"

bool Enemy_ActionState::AttackCheck()
{
	if (m_target.expired())return false;
	if (m_target.lock()->GetPlayer().expired())return false;

	KdCollider::SphereInfo sphereInfo;
	Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ m_target.lock()->GetParam().ForwardX,m_target.lock()->GetParam().ForwardY,m_target.lock()->GetParam().ForwardZ }, nowRotY);
	nowVec.Normalize();
	sphereInfo.m_sphere.Center = m_target.lock()->GetPos() + nowVec * (m_target.lock()->GetParam().AtkRange / 2.0f);
	sphereInfo.m_sphere.Radius = m_target.lock()->GetParam().AtkRange / 2.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	if (m_target.lock()->GetPlayer().lock()->Intersects(sphereInfo, nullptr))
	{
		return true;
	}

	return false;
}