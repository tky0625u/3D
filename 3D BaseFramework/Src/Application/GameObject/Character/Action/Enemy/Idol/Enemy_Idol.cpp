#include "Enemy_Idol.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../Enemy/EnemyBase.h"
#include"../Enemy_ConText.h"

#include"../Run/Enemy_Run.h"
#include"../Attack/Enemy_Attack.h"
#include"../Hit/Enemy_Hit.h"
#include"../Stumble/Enemy_Stumble.h"

void Enemy_Idol::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Idol")
		{
			m_target.lock()->SetAnime("Idol", true, 1.0f);
			return;
		}

		Event();
	}
}

void Enemy_Idol::Event()
{
	if (ChaseCheck())m_target.lock()->GetConText()->Run();
}

void Enemy_Idol::Run(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Run> run = std::make_shared<Enemy_Run>();
	if (m_target.expired())return;
	run->SetTarget(m_target.lock());
	context->SetState(run);
}

void Enemy_Idol::Attack(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Attack> attack = std::make_shared<Enemy_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	context->SetState(attack);
}

void Enemy_Idol::Hit(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	context->SetState(hit);
}

void Enemy_Idol::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Stumble> stumble = std::make_shared<Enemy_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	context->SetState(stumble);
}
