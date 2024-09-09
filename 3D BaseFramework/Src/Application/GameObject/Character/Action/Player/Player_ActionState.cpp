#include "Player_ActionState.h"
#include"../../../ObjectManager.h"
#include"../../Enemy/EnemyBase.h"
#include"../Enemy/Enemy_ConText.h"
#include"../../Player/Player.h"

void Player_ActionState::AttackDamage()
{
	if (m_target.expired())return;

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_target.lock()->GetWeaponMat().Translation();
	sphereInfo.m_sphere.Radius = 10.0f;
	sphereInfo.m_type = KdCollider::TypeDamage;

	for (auto& sphere : ObjectManager::Instance().GetEnemyList())
	{
		if (sphere->Intersects(sphereInfo, nullptr))
		{
			sphere->GetConText()->Hit(m_target.lock()->GetParam().Atk);
		}
	}
}
