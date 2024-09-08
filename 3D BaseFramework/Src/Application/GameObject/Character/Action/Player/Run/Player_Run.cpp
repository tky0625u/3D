#include "Player_Run.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../../../../Camera/CameraBase.h"

#include"../Idol/Player_Idol.h"
#include"../Attack/Player_Attack.h"
#include"../Roll/Player_Roll.h"
#include"../Guard/Player_Guard.h"
#include"../Hit/Player_Hit.h"

void Player_Run::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToRun")
		{
			m_target.lock()->SetAnime("IdolToRun", false, 3.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			m_ChangeFlg = true;
			return;
		}

		if (!(m_ActionType & Player::ActionType::MoveType))
		{
			m_flow = Flow::EndType;
			return;
		}

		Event();
	}
}

void Player_Run::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Run")
		{
			m_target.lock()->SetAnime("Run", true, 1.0f);
			return;
		}

		if (!(m_ActionType & Player::ActionType::MoveType))
		{
			m_flow = Flow::EndType;
			return;
		}

		Event();
	}
}

void Player_Run::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "RunToIdol")
		{
			m_target.lock()->SetAnime("RunToIdol", false, 3.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			Idol(m_conText);
			return;
		}

		if (m_ActionType & Player::ActionType::MoveType)
		{
			m_flow = Flow::StartType;
			return;
		}
	}
}

void Player_Run::Event()
{
	std::shared_ptr<CharacterBase> _player = nullptr;
	if (m_target.expired() == false)_player = m_target.lock();
	Math::Vector3 dir = Math::Vector3::Zero;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		dir.z = 1.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		dir.z = -1.0f;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		dir.x = -1.0f;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		dir.x = 1.0f;
	}
	CameraTransform(dir);
	if(m_target.expired()==false)Rotate(dir,m_target.lock());
	_player->SetMove(dir);
	if (m_flow == Flow::EndType)m_flow = Flow::CenterType;
}

void Player_Run::CameraTransform(Math::Vector3& _dir)
{
	Math::Matrix cameraRotYMat = Math::Matrix::Identity;
	if (m_target.lock()->GetCamera().expired() == false)
	{
		cameraRotYMat = m_target.lock()->GetCamera().lock()->GetRotationYMatrix();
	}
	_dir = _dir.TransformNormal(_dir, cameraRotYMat);

	_dir.Normalize(); //正規化
}

void Player_Run::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Player_Run::Attack(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Attack> attack = std::make_shared<Player_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	context->SetState(attack);
}

void Player_Run::Guard(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Guard> guard = std::make_shared<Player_Guard>();
	if (m_target.expired())return;
	guard->SetTarget(m_target.lock());
	context->SetState(guard);
}

void Player_Run::Roll(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Roll> roll = std::make_shared<Player_Roll>();
	if (m_target.expired())return;
	roll->SetTarget(m_target.lock());
	context->SetState(roll);
}

void Player_Run::Hit(std::shared_ptr<Player_ActionConText> context, int _damage, Math::Vector3 _pos)
{
	std::shared_ptr<Player_Hit> hit = std::make_shared<Player_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	context->SetState(hit);
	m_target.lock()->GetParam().Hp -= _damage;
	if (m_target.lock()->GetParam().Hp <= 0)m_target.lock()->GetParam().Hp = 0;
}
