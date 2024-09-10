#include "Enemy_Attack.h"
#include"../../../Enemy/EnemyBase.h"
#include"../../../Player/Player.h"
#include"../Enemy_ConText.h"
#include"../../Player/Player_ConText.h"

void Enemy_Attack::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolTOAttack")
		{
			m_target.lock()->SetAnime("IdolTOAttack", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Enemy_Attack::Center()
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

		Event();
	}
}

void Enemy_Attack::End()
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
			AttackCheck(m_atkFlg);
			if (m_atkFlg) { m_flow = Flow::StartType; }
			else { m_target.lock()->GetConText()->Idol(); }
			return;
		}
	}
}

void Enemy_Attack::Event()
{
	if (m_target.expired())return;
	if (m_target.lock()->GetPlayer().expired())return;

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_target.lock()->GetPos();
	sphereInfo.m_sphere.Radius = 100.0f;
	sphereInfo.m_type = KdCollider::TypeDamage;
	
	if (m_target.lock()->GetPlayer().lock()->Intersects(sphereInfo, nullptr))
	{
		m_target.lock()->GetPlayer().lock()->GetConText()->Hit(m_target.lock()->GetParam().Atk,m_target.lock());
	}
}