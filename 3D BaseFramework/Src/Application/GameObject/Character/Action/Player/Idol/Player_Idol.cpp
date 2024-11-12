#include "Player_Idol.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../../ObjectManager.h"
#include"../../../../StageManager.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

void Player_Idol::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Idol")
		{
			m_target.lock()->SetAnime("Idol", true, 1.0f);
			return;
		}
	}
}

void Player_Idol::Init()
{
	m_flow = Flow::CenterType;
}

void Player_Idol::ChangeAction()
{
	if (m_KeyType & Player_ActionConText::KeyType::MoveKey)
	{
		m_target.lock()->GetConText()->Run();
	}
	else if (m_KeyType & Player_ActionConText::KeyType::AttackKey && !(m_target.lock()->GetConText()->GetBeforeKeyType() & Player_ActionConText::KeyType::AttackKey))
	{
		m_target.lock()->GetConText()->Attack();
	}
	else if (m_KeyType & Player_ActionConText::KeyType::GuardKey)
	{
		m_target.lock()->GetConText()->Guard();
	}
	else if (m_KeyType & Player_ActionConText::KeyType::RollKey && !(m_target.lock()->GetConText()->GetBeforeKeyType() & Player_ActionConText::KeyType::RollKey))
	{
		m_target.lock()->GetConText()->Roll();
	}
}