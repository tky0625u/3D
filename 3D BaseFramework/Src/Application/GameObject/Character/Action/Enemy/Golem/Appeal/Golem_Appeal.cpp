#include "Golem_Appeal.h"
#include"../../../../Enemy/golem/Golem.h"
#include"../../Enemy_ConText.h"

void Golem_Appeal::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Appeal")
		{
			m_target.lock()->SetAnime("Appeal", false, 1.0f);
			m_Effect = KdEffekseerManager::GetInstance().Play("Enemy/MagicDrak.efkefc", m_target.lock()->GetPos(), 5.0f, 1.0f, false);
			return;
		}

		if (m_target.lock()->GetIsAnimator() && (m_Effect.expired() || !m_Effect.lock()->IsPlaying()))
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}
