#include "Player_Hit.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

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
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Player_Hit::Init()
{

}
