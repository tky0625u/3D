#include "Golem_Crushing.h"
#include"../../../../Enemy/Golem/Golem.h"
#include"../../Enemy_ConText.h"

void Golem_Crushing::Start()
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
			return;
		}

		CrushingAction();
	}
}

void Golem_Crushing::CrushingAction()
{
	m_target.lock()->CrushingAction();
}
