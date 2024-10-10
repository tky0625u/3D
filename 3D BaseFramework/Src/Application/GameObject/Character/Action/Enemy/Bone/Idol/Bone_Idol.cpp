#include "Bone_Idol.h"
#include"../../../../Enemy/Bone/Bone.h"
#include"../../Enemy_ConText.h"

void Bone_Idol::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Idol")
		{
			m_target.lock()->SetAnime("Idol", true, 1.0f);
			return;
		}

		m_target.lock()->GetConText()->Run();
	}
}

void Bone_Idol::Init()
{
	m_flow = Flow::CenterType;
}
