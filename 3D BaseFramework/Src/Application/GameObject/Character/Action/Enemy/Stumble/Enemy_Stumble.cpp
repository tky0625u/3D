#include "Enemy_Stumble.h"
#include"../../../CharacterBase.h"

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
			if (m_target.expired() == false)m_target.lock()->SetNextAction("Idol");
			Reset();
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
