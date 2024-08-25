#include "Player_Roll.h"
#include"../../../Player/Player.h"

void Player_Roll::End()
{
	Roll();

	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Roll")
		{
			m_target.lock()->SetAnime("Roll", false, 2.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)
			{
				m_target.lock()->SetNextAction("Idol");
				m_target.lock()->InviOFF();
			}
			m_end = true;
			return;
		}
	}
}

void Player_Roll::Roll()
{
	if (m_target.expired() == false)m_target.lock()->InviON();

	Math::Vector3 dir = Math::Vector3::Zero;
	std::shared_ptr<CharacterBase> player;
	if(m_target.expired()==false)player = m_target.lock();

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(player->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(player->GetParam().ForwardX, player->GetParam().ForwardY, player->GetParam().ForwardZ), nowRot);
	dir = nowVec;
	dir.Normalize();

	player->SetMove(dir, true);
}

void Player_Roll::Init()
{
	m_ChangeFlg = true;
	m_flow = Flow::EndType;
	m_end = false;
}
