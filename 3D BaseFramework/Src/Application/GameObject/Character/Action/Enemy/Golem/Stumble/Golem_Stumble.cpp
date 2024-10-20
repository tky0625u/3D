#include "Golem_Stumble.h"
#include"../../../../Enemy/golem/Golem.h"
#include"../../Enemy_ConText.h"

void Golem_Stumble::Start()
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

		Stumble();
	}
}

void Golem_Stumble::End()
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

void Golem_Stumble::Stumble()
{
	Math::Matrix _nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetAngle()));
	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(Math::Vector3{ m_target.lock()->GetForward().x, m_target.lock()->GetForward().y, m_target.lock()->GetForward().z }, _nowRot);
	_nowVec.y = 0.0f;
	_nowVec *= -1.0f;
	_nowVec.Normalize();
	m_target.lock()->SetMove(_nowVec, 0.1f);
}
