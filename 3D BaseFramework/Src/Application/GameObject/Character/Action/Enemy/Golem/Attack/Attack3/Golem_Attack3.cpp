#include "Golem_Attack3.h"
#include"../../../Enemy_ConText.h"
#include"../../../../../Enemy/golem/Golem.h"
#include"../../../../../Player/Player.h"
#include"../../../../../Action/Player/Player_ConText.h"

void Golem_Attack3::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToAttack3")
		{
			m_target.lock()->SetAnime("IdolToAttack3", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Golem_Attack3::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack3")
		{
			m_target.lock()->SetAnime("Attack3", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			AttackDamage();
			KdEffekseerManager::GetInstance().Play("Enemy/Golem/smash.efkefc", m_target.lock()->GetPos(), Math::Vector3{ 50.0f,15.0f,50.0f }, 1.0f, false);
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Golem_Attack3::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack3ToIdol")
		{
			m_target.lock()->SetAnime("Attack3ToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}

		Math::Vector3 dir = m_target.lock()->GetTarget().lock()->GetPos() - m_target.lock()->GetPos();
		dir.y = 0.0f;
		dir.Normalize();
		Rotate(dir, m_target.lock(),2.5f);
	}
}

void Golem_Attack3::AttackDamage()
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_target.lock()->GetQuakePoint().Translation();
	sphereInfo.m_sphere.Radius = 60.0f;
	sphereInfo.m_type = KdCollider::Type::TypeDamage;

	Math::Color _color = { 1,0,0,1 };
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, _color);

	if (m_target.lock()->GetTarget().lock()->Intersects(sphereInfo, nullptr))
	{
		m_target.lock()->GetTarget().lock()->GetConText()->Hit(5, m_target.lock());
	}
}

void Golem_Attack3::Hit(int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
}
