#include "Enemy_Stumble.h"
#include"../../../Enemy/EnemyBase.h"
#include"../Enemy_ConText.h"

#include"../Idol/Enemy_Idol.h"
#include"../Hit/Enemy_Hit.h"

void Enemy_Stumble::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Stumble")
		{
			m_target.lock()->SetAnime("Stumble", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Enemy_Stumble::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "StumbleToIdol")
		{
			m_target.lock()->SetAnime("StumbleToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Enemy_Stumble::Stumble()
{
	std::shared_ptr<CharacterBase> target = nullptr;
	if (m_target.expired() == false)
	{
		target = m_target.lock();
	}

	Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(target->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ target->GetParam().ForwardX,target->GetParam().ForwardY,target->GetParam().ForwardZ }, nowRotY);
	nowVec *= -1;
	nowVec.y = 0;
	nowVec.Normalize();


}

void Enemy_Stumble::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Enemy_Stumble::Hit(std::shared_ptr<Enemy_ConText> context,int _damage)
{
	std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	context->SetState(hit);
	m_target.lock()->GetParam().Hp -= _damage;
}
