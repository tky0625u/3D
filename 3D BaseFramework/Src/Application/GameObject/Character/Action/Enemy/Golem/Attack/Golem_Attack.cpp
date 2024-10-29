#include "Golem_Attack.h"
#include"../../../../../ObjectManager.h"
#include"../../Enemy_ConText.h"
#include"../../../../Enemy/golem/Golem.h"
#include"../../../../Player/Player.h"
#include"../../../Player/Player_ConText.h"
#include"../../../../../Weapon/Sword/Sword.h"

void Golem_Attack::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToAttack" && m_target.lock()->GetAnime() != "AttackStart")
		{
			m_target.lock()->SetAnime("IdolToAttack", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.lock()->GetAnime() == "IdolToAttack")
			{
				m_target.lock()->SetAnime("AttackStart", false, 0.5f);
				KdEffekseerManager::GetInstance().Play("Enemy/BloodLance.efkefc", m_target.lock()->GetAttackStartPointMat().Translation(), 0.3f, 2.0f, false);
				return;
			}
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Golem_Attack::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack")
		{
			m_target.lock()->SetAnime("Attack", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}

		Attack();
	}
}

void Golem_Attack::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "AttackToIdol")
		{
			m_target.lock()->SetAnime("AttackToIdol", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			//AttackCheck(m_atkFlg);
			if (m_atkFlg) { m_flow = Flow::StartType; }
			else { m_target.lock()->GetConText()->Idol(); }
			return;
		}
	}
}

void Golem_Attack::Attack()
{
	if (m_target.expired())return;
	if (m_ObjManager.lock()->GetPlayer().expired())return;

	std::vector<KdCollider::SphereInfo> sphereInfoList;
	KdCollider::SphereInfo sphereInfo;
	if (m_target.lock()->GetSword().expired() == false)
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelTop().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelCenter().Translation();
		sphereInfoList.push_back(sphereInfo);

		sphereInfo.m_sphere.Center = m_target.lock()->GetSword().lock()->GetModelBottom().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	else
	{
		sphereInfo.m_sphere.Center = m_target.lock()->GetSwordMat().Translation();
		sphereInfoList.push_back(sphereInfo);
	}
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeDamage;

	for (int i = 0; i < sphereInfoList.size(); ++i)
	{
		if (m_ObjManager.lock()->GetPlayer().lock()->Intersects(sphereInfo, nullptr))
		{
			m_ObjManager.lock()->GetPlayer().lock()->GetConText()->Hit(m_target.lock()->GetParam().Atk, m_target.lock());
		}
	}
}