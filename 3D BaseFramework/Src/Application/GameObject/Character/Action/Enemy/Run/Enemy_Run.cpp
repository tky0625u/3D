#include "Enemy_Run.h"
#include"../../../Enemy/Bone/Bone.h"
#include"../../../../../Scene/SceneManager.h"
#include"../Enemy_ConText.h"

#include"../Idol/Enemy_Idol.h"
#include"../Attack/Enemy_Attack.h"
#include"../Hit/Enemy_Hit.h"
#include"../Stumble/Enemy_Stumble.h"

void Enemy_Run::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToRun")
		{
			m_target.lock()->SetAnime("IdolToRun", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}

		Event();
	}
}

void Enemy_Run::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Run")
		{
			m_target.lock()->SetAnime("Run", true, 0.7f);
			return;
		}

		Event();
	}
}

void Enemy_Run::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "RunToIdol")
		{
			m_target.lock()->SetAnime("RunToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)
			{
				if (!m_atkFlg)Idol(m_conText);
				else{Attack(m_conText);}
			}
			return;
		}
	}
}

void Enemy_Run::Event()
{
	if (!ChaseCheck())
	{
		m_flow = Flow::EndType;
		return;
	}
	if (AttackCheck())return;

	
}

bool Enemy_Run::AttackCheck()
{
	KdCollider::RayInfo rayInfo;
	Math::Matrix nowRotY = Math::Matrix::CreateRotationY(m_target.lock()->GetParam().Angle);
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ m_target.lock()->GetParam().ForwardX,m_target.lock()->GetParam().ForwardY,m_target.lock()->GetParam().ForwardZ }, nowRotY);
	nowVec.Normalize();
	rayInfo.m_dir = nowVec;
	rayInfo.m_pos = m_target.lock()->GetPos();
	rayInfo.m_range = m_target.lock()->GetParam().AtkRange;
	rayInfo.m_type = KdCollider::TypeBump;

	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->GetObjType() == ObjType::oPlayer)
		{
			m_flow = Flow::EndType;
			m_atkFlg = true;
			return m_atkFlg;
		}
	}

	m_atkFlg = false;
	return m_atkFlg;
}

void Enemy_Run::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	idol->SetConText(context);
	context->SetState(idol);
}

void Enemy_Run::Attack(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Attack> attack = std::make_shared<Enemy_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	attack->SetConText(context);
	context->SetState(attack);
}

void Enemy_Run::Hit(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	hit->SetConText(context);
	context->SetState(hit);
}

void Enemy_Run::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Stumble> stumble = std::make_shared<Enemy_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	stumble->SetConText(context);
	context->SetState(stumble);
}
