#include "Enemy_Run.h"
#include"../../../CharacterBase.h"
#include"../../../../../Scene/SceneManager.h"

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
			m_ChangeFlg = true;
			return;
		}
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

		Chase();
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
				if(!m_atkFlg)m_target.lock()->SetNextAction("Idol");
				else
				{
					m_target.lock()->SetNextAction("Attack"); 
					m_atkFlg = false;
				}
			}
			Reset();
			return;
		}
	}
}

void Enemy_Run::Chase()
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
		//if (ret->GetObjType() == ObjType::oPlayer)
		//{
		//	m_flow = Flow::EndType;
		//	m_ChangeFlg = true;
		//	m_atkFlg = true;
		//	return;
		//}
	}
}
