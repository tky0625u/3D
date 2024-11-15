#include "Player_Crushing.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../../../../ObjectManager.h"

void Player_Crushing::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Death")
		{
			m_target.lock()->SetAnime("Death", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->Expired();
			m_ObjectManager.lock()->SetGameOverParam();
			return;
		}

		CrushingAction();
	}
}

void Player_Crushing::CrushingAction()
{
	m_target.lock()->CrushingAction();
}
