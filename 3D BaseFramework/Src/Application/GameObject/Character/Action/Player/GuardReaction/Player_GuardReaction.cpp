#include "Player_GuardReaction.h"
#include"../../../CharacterBase.h"

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
			if (m_target.expired() == false)
			{
				m_target.lock()->SetNextAction("Guard");
				m_target.lock()->ChangeAction("Guard");
			}
			Reset();
			return;
		}
	}
}
