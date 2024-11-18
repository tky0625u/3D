#include "Golem_Attack1.h"
#include"../../../Enemy_ConText.h"
#include"../../../../../Enemy/golem/Golem.h"

void Golem_Attack1::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToAttack1")
		{
			m_target.lock()->SetAnime("IdolToAttack1", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Golem_Attack1::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack1")
		{
			m_target.lock()->SetAnime("Attack1", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Golem_Attack1::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Attack1ToIdol")
		{
			m_target.lock()->SetAnime("Attack1ToIdol", false, 0.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Golem_Attack1::Hit(int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
}
