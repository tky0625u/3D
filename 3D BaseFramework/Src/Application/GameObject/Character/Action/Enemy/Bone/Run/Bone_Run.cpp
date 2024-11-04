#include "Bone_Run.h"
#include"../../../../../ObjectManager.h"
#include"../../../../../../Scene/SceneManager.h"
#include"../../Enemy_ConText.h"
#include"../../../../Enemy/Bone/Bone.h"
#include"../../../../Enemy/EnemyManager.h"
#include"../../../../Player/Player.h"
#include"../../../../../../main.h"
void Bone_Run::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToRun")
		{
			m_target.lock()->SetAnime("IdolToRun", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Bone_Run::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Run")
		{
			m_target.lock()->SetAnime("Run", true, 0.7f);
			return;
		}
	}
}

void Bone_Run::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "RunToIdol")
		{
			m_target.lock()->SetAnime("RunToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)
			{
				m_target.lock()->GetConText()->Idol();
			}
			return;
		}
	}
}

void Bone_Run::Chace()
{


}
