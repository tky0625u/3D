#include "Player_Hit.h"
#include"../../../CharacterBase.h"

void Player_Hit::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Hit")
		{
			m_target.lock()->SetAnime("Hit", false, 1.5f);
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
