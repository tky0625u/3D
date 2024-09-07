#include "Enemy_Attack.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../Enemy/EnemyBase.h"
#include"../Enemy_ConText.h"

#include"../Idol/Enemy_Idol.h"
#include"../Hit/Enemy_Hit.h"
#include"../Stumble/Enemy_Stumble.h"

void Enemy_Attack::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolTOAttack")
		{
			m_target.lock()->SetAnime("IdolTOAttack", false, 1.5f);
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
		m_target.lock()->Attack(m_target.lock()->GetObjType());

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

		//Event();
	}
}

void Enemy_Attack::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "AttackToIdol")
		{
			m_target.lock()->SetAnime("AttackToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			Idol(m_conText);
			return;
		}
	}
}

void Enemy_Attack::Event()
{
	if (m_target.expired())return;
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_target.lock()->GetWeaponMat().Translation();
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeDamage;

	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->GetObjType() == ObjType::oPlayer)
		{
			if (ret->Intersects(sphereInfo, &retSphereList))
			{

			}
		}
	}
}

void Enemy_Attack::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	idol->SetConText(context);
	context->SetState(idol);
}

void Enemy_Attack::Hit(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	hit->SetConText(context);
	context->SetState(hit);
}

void Enemy_Attack::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Stumble> stumble = std::make_shared<Enemy_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	stumble->SetConText(context);
	context->SetState(stumble);
}
