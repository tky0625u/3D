#include "Bone_Hit.h"
#include"../../../../Enemy/Bone/Bone.h"
#include"../../Enemy_ConText.h"
#include"../../../../../ObjectManager.h"

void Bone_Hit::Start()
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

void Bone_Hit::Hit(int _damage)
{
	if (m_target.expired())return;
	m_target.lock()->SetAnime("Hit", false, 1.5f);
	m_target.lock()->Hit(_damage);
	if (m_target.lock()->GetParam().Hp <= 0)
	{
		Crushing();
		return;
	}
}
