#include "Enemy_ActionState.h"
#include"../../../../Scene/SceneManager.h"
#include"../../Enemy/EnemyBase.h"

bool Enemy_ActionState::ChaseCheck()
{
	if (m_target.expired())return false;

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_target.lock()->GetPos();
	sphereInfo.m_sphere.Radius = m_target.lock()->GetChaseRange();
	sphereInfo.m_type = KdCollider::TypeEvent;

	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->Intersects(sphereInfo, &retSphereList))
		{
			return true;
		}
	}

	return false;
}
