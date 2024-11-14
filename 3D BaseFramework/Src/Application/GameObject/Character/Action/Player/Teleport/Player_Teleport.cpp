#include "Player_Teleport.h"
#include"../../../../../Scene/SceneManager.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../Player/Player.h"

void Player_Teleport::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToTeleport")
		{
			m_target.lock()->SetAnime("IdolToTeleport", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}
	}
}

void Player_Teleport::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Teleport")
		{
			m_target.lock()->SetAnime("Teleport", true, 1.0f);
			m_target.lock()->SetDissolve(1.0f);
			m_handle = KdEffekseerManager::GetInstance().Play("Player/LightEnd.efkefc", m_target.lock()->GetPos(), m_target.lock()->GetSize(), 1.0f, false).lock()->GetHandle();
			return;
		}

		if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle))SceneManager::Instance().BlackAlphaChange(0.01f, true);
	}
}

void Player_Teleport::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "TeleportToIdol")
		{
			m_target.lock()->SetAnime("TeleportToIdol", false, 1.0f);
			m_target.lock()->SetDissolve(0.0f);
			KdEffekseerManager::GetInstance().Play("Player/LightEnd.efkefc", m_target.lock()->GetPos(), m_target.lock()->GetSize(), 1.0f, false);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			Idol();
			return;
		}
	}
}
