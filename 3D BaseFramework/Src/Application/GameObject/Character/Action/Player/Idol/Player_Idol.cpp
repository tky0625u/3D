#include "Player_Idol.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Run/Player_Run.h"
#include"../Attack/Player_Attack.h"
#include"../Roll/Player_Roll.h"
#include"../Guard/Player_Guard.h"
#include"../Hit/Player_Hit.h"

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

void Player_Idol::Run(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Run> run = std::make_shared<Player_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	run->SetCamera(_camera);
	context->SetState(run);
}

void Player_Idol::Attack(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Attack> attack = std::make_shared<Player_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	context->SetState(attack);
}

void Player_Idol::Guard(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	context->SetState(guard);
}

void Player_Idol::Roll(std::shared_ptr<Player_ActionConText> context, std::weak_ptr<CameraBase> _camera)
{
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	if (m_target.expired())return;
	roll->SetTarget(m_target.lock());
	roll->SetCamera(_camera);
	context->SetState(roll);
}

void Player_Idol::Hit(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	context->SetState(hit);
}
