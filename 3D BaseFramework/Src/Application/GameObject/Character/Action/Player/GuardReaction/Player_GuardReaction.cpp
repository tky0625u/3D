#include "Player_GuardReaction.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Guard/Player_Guard.h"

void Player_GuardReaction::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "GuardReaction")
		{
			m_target.lock()->SetAnime("GuardReaction", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Guard();
			return;
		}
	}
}

void Player_GuardReaction::Init()
{
	m_staminaRecoveryFlg = false;
}

void Player_GuardReaction::Guard(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	guard->SetFlow(Flow::CenterType);
	context->SetState(guard);
	m_target.lock()->SetNextState(guard);
}
