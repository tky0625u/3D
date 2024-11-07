#include "Bone_Crushing.h"
#include"../../../../Enemy/Bone/Bone.h"
#include"../../Enemy_ConText.h"

void Bone_Crushing::Start()
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

		m_target.lock()->CrushingAction();
	}
}

void Bone_Crushing::CrushingAction()
{
	
}
