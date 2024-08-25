#include "Player_Guard.h"
#include"../../../Player/Player.h"

void Player_Guard::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToGuard")
		{
			m_target.lock()->SetAnime("IdolToGuard", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Player_Guard::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Guard")
		{
			m_target.lock()->SetAnime("Guard", true, 1.0f);
			return;
		}
	}
}

void Player_Guard::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "GuardToIdol")
		{
			m_target.lock()->SetAnime("GuardToIdol", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)m_target.lock()->SetNextAction("Idol");
			m_end = true;
			return;
		}
	}
}

void Player_Guard::KeyCheck(const UINT key, const UINT before)
{
	if (!m_ChangeFlg)return;

	if (m_end)
	{
		Reset();
		return;
	}

	if (!(key & Player::ActionType::Guard))
	{
		m_flow = Flow::EndType;
	}
}

void Player_Guard::Init()
{
	m_ChangeFlg = true;
}

void Player_Guard::Reset()
{
	m_flow = Flow::StartType;
	m_end = false;
	m_ChangeFlg = true;
}
