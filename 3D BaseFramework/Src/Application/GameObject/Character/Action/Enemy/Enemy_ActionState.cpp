#include "Enemy_ActionState.h"
#include"../../../ObjectManager.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../../main.h"
#include"../../Enemy/EnemyBase.h"
#include"../../Player/Player.h"
#include"Enemy_ConText.h"

void Enemy_ActionState::AttackCheck()
{
	if (m_target.expired())return;
	if (m_ObjManager.lock()->GetPlayer().expired())return;

	if (m_target.lock()->GetAtkRange() < (m_target.lock()->GetPos() - m_target.lock()->GetPos()).LengthSquared())return;

	KdCollider::RayInfo rayInfo;
	Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetAngle().y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ m_target.lock()->GetForward().x,m_target.lock()->GetForward().y,m_target.lock()->GetForward().z}, nowRotY);
	nowVec.Normalize();
	rayInfo.m_pos = m_target.lock()->GetPos();
	rayInfo.m_pos.y = (m_ObjManager.lock()->GetPlayer().lock()->GetEnemyAttackPointMat().Translation().y);
	rayInfo.m_dir = nowVec;
	rayInfo.m_range = m_target.lock()->GetAtkRange();
	rayInfo.m_type = KdCollider::Type::TypeBump;

	std::list<KdCollider::CollisionResult> _List;
	if (m_ObjManager.lock()->GetPlayer().lock()->Intersects(rayInfo, &_List))
	{
		m_flow = Flow::EndType;
		return;
	}
}