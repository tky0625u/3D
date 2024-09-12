#include "Enemy_Idol.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../Enemy/EnemyBase.h"
#include"../Enemy_ConText.h"

void Enemy_Idol::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Idol")
		{
			m_target.lock()->SetAnime("Idol", true, 1.0f);
			return;
		}

		if (ChaseCheck())m_target.lock()->GetConText()->Run();
	}
}