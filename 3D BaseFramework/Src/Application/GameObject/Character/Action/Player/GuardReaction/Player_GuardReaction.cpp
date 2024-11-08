#include "Player_GuardReaction.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Guard/Player_Guard.h"

void Player_GuardReaction::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "GuardReaction")
		{
			m_target.lock()->SetAnime("GuardReaction", false, 1.0f);
			KdAudioManager::Instance().Play("Asset/Sound/Game/SE/Player/ロボットを殴る1.WAV", 0.05f, false);
			KdEffekseerManager::GetInstance().Play("Player/Spark.efkefc", m_target.lock()->GetShieldMat().Translation(), 1.0f, 1.0f, false);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_target.lock()->GetConText()->Guard();
			return;
		}
	}
}

void Player_GuardReaction::Init()
{

}

void Player_GuardReaction::Guard()
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	guard->SetFlow(Flow::CenterType);
	guard->SetGuardTime(m_guardTime);
	guard->SetObjectManager(m_ObjManager.lock());
	m_target.lock()->SetNextState(guard, Player::Action::GuardType);
}
