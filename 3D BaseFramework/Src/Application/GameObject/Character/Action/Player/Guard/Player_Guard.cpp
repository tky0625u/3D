#include "Player_Guard.h"
#include"../../../Player/Player.h"
#include"../Player_ConText.h"

#include"../Idol/Player_Idol.h"
#include"../GuardReaction/Player_GuardReaction.h"
#include"../Parry/Player_Parry.h"

void Player_Guard::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToGuard")
		{
			m_target.lock()->SetAnime("IdolToGuard", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}

		if (!(m_ActionType & Player::ActionType::GuardType))
		{
			m_flow = Flow::EndType;
			return;
		}

		Event();
	}
}

void Player_Guard::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Guard")
		{
			m_target.lock()->SetAnime("Guard", true, 1.0f);
			return;
		}

		if (!(m_ActionType & Player::ActionType::GuardType))
		{
			m_flow = Flow::EndType;
			return;
		}

		Event();
	}
}

void Player_Guard::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "GuardToIdol")
		{
			m_target.lock()->SetAnime("GuardToIdol", false, 1.0f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			Idol(m_conText);
			return;
		}

		if (m_ActionType & Player::ActionType::GuardType)
		{
			m_flow = Flow::StartType;
			return;
		}
	}
}

void Player_Guard::Init()
{
	m_ChangeFlg = true;
}

void Player_Guard::Event()
{
	m_guardTime++;
}

void Player_Guard::GuardRotate(Math::Vector3 _pos)
{
	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(m_target.lock()->GetParam().ForwardX, m_target.lock()->GetParam().ForwardY, m_target.lock()->GetParam().ForwardZ), nowRot);

	//向きたい方向
	Math::Vector3 toVec = _pos - m_target.lock()->GetPos();
	toVec.Normalize();

	//内角 回転する角を求める
	float d = nowVec.Dot(toVec);
	d = std::clamp(d, -1.0f, 1.0f); //誤差修正

	//回転角度を求める
	float ang = DirectX::XMConvertToDegrees(acos(d));

	//角度変更
	//外角　どっち回転かを求める
	Math::Vector3 c = toVec.Cross(nowVec);
	if (c.y >= 0)
	{
		//右回転
		m_target.lock()->GetParam().Angle -= ang;
		if (m_target.lock()->GetParam().Angle < 0.0f)
		{
			m_target.lock()->GetParam().Angle += 360.0f;
		}
	}
	else
	{
		//左回転
		m_target.lock()->GetParam().Angle += ang;
		if (m_target.lock()->GetParam().Angle >= 360.0f)
		{
			m_target.lock()->GetParam().Angle -= 360.0f;
		}
	}
}

void Player_Guard::Idol(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Idol> idol = std::make_shared<Player_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	context->SetState(idol);
}

void Player_Guard::GuardReaction(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_GuardReaction> guardReaction = std::make_shared<Player_GuardReaction>();
	if (m_target.expired())return;
	guardReaction->SetTarget(m_target.lock());
	context->SetState(guardReaction);
}

void Player_Guard::Parry(std::shared_ptr<Player_ActionConText> context)
{
	std::shared_ptr<Player_Parry> parry = std::make_shared<Player_Parry>();
	if (m_target.expired())return;
	parry->SetTarget(m_target.lock());
	context->SetState(parry);
}

void Player_Guard::Hit(std::shared_ptr<Player_ActionConText> context,int _damage,Math::Vector3 _pos)
{
	if (m_target.expired())return;

	GuardRotate(_pos);

	if (m_guardTime <= 30)m_target.lock()->GetConText()->Parry();
	else { m_target.lock()->GetConText()->GuardReaction(); }
}
