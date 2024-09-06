#include "Enemy_Hit.h"
#include"../../../CharacterBase.h"
#include"../Enemy_ConText.h"

#include"../Idol/Enemy_Idol.h"

void Enemy_Hit::Start()
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
			Idol(m_conText);
			return;
		}
	}

}

void Enemy_Hit::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	if (m_player.expired() == false)idol->SetPlayer(m_player.lock());
	context->SetState(idol);
}
