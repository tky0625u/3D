#include "Player_Roll.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"
#include"../Player_ActionState.h"

void Player_Roll::Start()
{
	if (m_target.expired() == false)
	{
		Roll();
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
			m_target.lock()->GetConText()->Idol();
			return;
		}
	}
}

void Player_Roll::Init()
{
	m_flow = Flow::StartType;
}

void Player_Roll::Roll()
{
	if (m_target.expired())return;

	Math::Vector3 dir = Math::Vector3::Zero;
	std::shared_ptr<CharacterBase> player;
	if (m_target.expired() == false)player = m_target.lock();

	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(player->GetAngle()));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(player->GetForward().x, player->GetForward().y, player->GetForward().z), nowRot);
	dir = nowVec;
	dir.Normalize();

	player->SetMove(dir, 2.5f);
}
