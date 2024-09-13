#include "Bone_Stumble.h"
#include"../../../../Enemy/Bone/Bone.h"
#include"../../Enemy_ConText.h"

void Bone_Stumble::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Stumble")
		{
			m_target.lock()->SetAnime("Stumble", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Bone_Stumble::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "StumbleToIdol")
		{
			m_target.lock()->SetAnime("StumbleToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}
