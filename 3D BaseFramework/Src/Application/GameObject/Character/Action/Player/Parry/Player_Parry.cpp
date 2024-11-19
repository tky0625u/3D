#include "Player_Parry.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../../ObjectManager.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../../Weapon/Shield/Shield.h"

void Player_Parry::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Parrying")
		{
			m_target.lock()->SetAnime("Parrying", false, 1.0f);
			KdEffekseerManager::GetInstance().Play("Player/hit_hanmado_0409.efkefc", m_target.lock()->GetShield().lock()->GetParryPoint().Translation(), 1.0f, 0.5f, false);
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/maou_se_magic_ice05.WAV", 0.1f, false);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Player_Parry::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "ParryingToIdol")
		{
			m_target.lock()->SetAnime("ParryingToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Idol();
			m_ObjectManager.lock()->SlowChange();
			return;
		}
	}
}

void Player_Parry::ChangeAction()
{
	//if (m_flow != Flow::EndType)return;

	if (m_KeyType & Player_ActionConText::KeyType::AttackKey && !(m_target.lock()->GetConText()->GetBeforeKeyType() & Player_ActionConText::KeyType::AttackKey))
	{
		if (m_target.lock()->GetParryID() != -1)m_target.lock()->GetConText()->Counter();
		else { m_target.lock()->GetConText()->Attack(); }
		m_ObjectManager.lock()->SlowChange();
		return;
	}

	if (m_flow != Flow::EndType)return;

	if (m_KeyType & Player_ActionConText::KeyType::MoveKey)
	{
		m_target.lock()->GetConText()->Run();
		m_ObjectManager.lock()->SlowChange();
	}
	else if (m_KeyType & Player_ActionConText::KeyType::GuardKey)
	{
		m_target.lock()->GetConText()->Guard();
		m_ObjectManager.lock()->SlowChange();
	}
	else if (m_KeyType & Player_ActionConText::KeyType::RollKey && !(m_target.lock()->GetConText()->GetBeforeKeyType() & Player_ActionConText::KeyType::RollKey))
	{
		m_target.lock()->GetConText()->Roll();
		m_ObjectManager.lock()->SlowChange();
	}
}