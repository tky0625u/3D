#include "Player_Roll.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Idol/Player_Idol.h"

void Player_Roll::Start()
{
	if (m_target.expired() == false)
	{
		Event();
		if (m_target.lock()->GetAnime() != "Roll")
		{
			m_target.lock()->SetAnime("Roll", false, 1.2f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::EndType;
			return;
		}
	}
}

void Player_Roll::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "RollToIdol")
		{
			m_target.lock()->SetAnime("RollToIdol", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)
			{
				m_target.lock()->InviOFF();
				Idol(m_conText);
			}
			return;
		}
	}
}

void Player_Roll::Event()
{
	if (m_target.expired() == false)m_target.lock()->InviON();

	Math::Vector3 dir = Math::Vector3::Zero;
	std::shared_ptr<CharacterBase> player;
	if (m_target.expired() == false)player = m_target.lock();

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(player->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(player->GetParam().ForwardX, player->GetParam().ForwardY, player->GetParam().ForwardZ), nowRot);
	dir = nowVec;
	dir.Normalize();

	Rotate(dir);
	player->SetMove(dir);
}

void Player_Roll::Init()
{
	m_ChangeFlg = true;
	m_flow = Flow::StartType;
	m_end = false;
}

void Player_Roll::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}
