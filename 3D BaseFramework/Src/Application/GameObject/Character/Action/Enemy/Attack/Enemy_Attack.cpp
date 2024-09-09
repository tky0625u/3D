#include "Enemy_Attack.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../Enemy/EnemyBase.h"
#include"../../../Player/Player.h"
#include"../Enemy_ConText.h"
#include"../../Player/Player_ConText.h"

#include"../Idol/Enemy_Idol.h"
#include"../Hit/Enemy_Hit.h"
#include"../Stumble/Enemy_Stumble.h"

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
	sphereInfo.m_sphere.Center = m_target.lock()->GetWeaponMat().Translation();
	sphereInfo.m_sphere.Radius = 10.0f;
	sphereInfo.m_type = KdCollider::TypeDamage;
	
	if (m_target.lock()->GetPlayer().lock()->Intersects(sphereInfo, nullptr))
	{
		m_target.lock()->GetPlayer().lock()->GetConText()->Hit(m_target.lock()->GetParam().Atk,m_target.lock());
	}
}

void Enemy_Attack::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Enemy_Attack::Hit(std::shared_ptr<Enemy_ConText> context,int _damage)
{
	std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	context->SetState(hit);
	m_target.lock()->GetParam().Hp -= _damage;
}

void Enemy_Attack::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Stumble> stumble = std::make_shared<Enemy_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	context->SetState(stumble);
}
