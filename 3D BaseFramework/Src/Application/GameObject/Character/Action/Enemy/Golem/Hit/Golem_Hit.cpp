#include "Golem_Hit.h"
#include"../../../../Enemy/golem/Golem.h"
#include"../../Enemy_ConText.h"
#include"../../../../../ObjectManager.h"

void Golem_Hit::Start()
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

void Golem_Hit::Hit(std::shared_ptr<Enemy_ConText> context, int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)return;
	ObjectManager::Instance().SetStopTime(10);
}
