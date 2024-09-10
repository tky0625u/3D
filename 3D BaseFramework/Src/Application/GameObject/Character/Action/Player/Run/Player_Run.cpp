#include "Player_Run.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"
#include"../../../../Camera/CameraBase.h"

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
			return;
		}

		if (!(m_ActionType & Player_ActionConText::ActionType::MoveType))
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

		if (!(m_ActionType & Player_ActionConText::ActionType::MoveType))
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
			m_target.lock()->GetConText()->Idol();
			return;
		}

		if (m_ActionType & Player_ActionConText::MoveType)
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

void Player_Run::ChangeAction()
{
	if (m_ActionType & Player_ActionConText::ActionType::AttackType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::AttackType))
	{
		m_target.lock()->GetConText()->Attack();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::GuardType)
	{
		m_target.lock()->GetConText()->Guard();
	}
	else if (m_ActionType & Player_ActionConText::ActionType::RollType && !(m_target.lock()->GetConText()->GetBeforeActionType() & Player_ActionConText::ActionType::RollType))
	{
		m_target.lock()->GetConText()->Roll();
	}
}