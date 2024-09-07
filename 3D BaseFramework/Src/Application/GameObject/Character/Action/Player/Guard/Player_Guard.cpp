#include "Player_Guard.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Idol/Player_Idol.h"
#include"../GuardReaction/Player_GuardReaction.h"
#include"../Parry/Player_Parry.h"

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

		if (!(m_ActionType & Player::ActionType::GuardType))
		{
			m_flow = Flow::EndType;
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

		if (!(m_ActionType & Player::ActionType::GuardType))
		{
			m_flow = Flow::EndType;
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
			Idol(m_conText);
			return;
		}

		if (m_ActionType & Player::ActionType::GuardType)
		{
			m_flow = Flow::StartType;
			return;
		}
	}
}

void Player_Guard::Init()
{
	m_ChangeFlg = true;
}

void Player_Guard::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Player_Guard::GuardReaction(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_GuardReaction> guardReaction = std::make_shared<Player_GuardReaction>();
	if (m_target.expired())return;
	guardReaction->SetTarget(m_target.lock());
	context->SetState(guardReaction);
}

void Player_Guard::Parry(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Parry> parry = std::make_shared<Player_Parry>();
	if (m_target.expired())return;
	parry->SetTarget(m_target.lock());
	context->SetState(parry);
}
