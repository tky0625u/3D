//#include "Player_Guard.h"
//#include"../../../../../Scene/SceneManager.h"
//#include"../../../../ObjectManager.h"
//#include"../../../Enemy/EnemyBase.h"
//#include"../../../BulletBase/BulletBase.h"
//#include"../../Enemy/Enemy_ConText.h"
//#include"../../../Player/Player.h"
//#include"../Player_ConText.h"
//#include"../Player_ActionState.h"
//
//void Player_Guard::Start()
//{
//	if (m_target.expired() == false)
//	{
//		if (m_target.lock()->GetAnime() != "IdolToGuard")
//		{
//			m_target.lock()->SetAnime("IdolToGuard", false, 1.0f);
//			return;
//		}
//
//		if (m_target.lock()->GetIsAnimator())
//		{
//			m_flow = Flow::CenterType;
//			return;
//		}
//
//		if (!(m_KeyType & Player_ActionConText::KeyType::GuardKey))
//		{
//			m_flow = Flow::EndType;
//			return;
//		}
//
//		m_guardTime++;
//	}
//}
//
//void Player_Guard::Center()
//{
//	if (m_target.expired() == false)
//	{
//		if (m_target.lock()->GetAnime() != "Guard")
//		{
//			m_target.lock()->SetAnime("Guard", true, 1.0f);
//			return;
//		}
//
//		if (!(m_KeyType & Player_ActionConText::KeyType::GuardKey))
//		{
//			m_flow = Flow::EndType;
//			return;
//		}
//
//		m_guardTime++;
//	}
//}
//
//void Player_Guard::End()
//{
//	if (m_target.expired() == false)
//	{
//		if (m_target.lock()->GetAnime() != "GuardToIdol")
//		{
//			m_target.lock()->SetAnime("GuardToIdol", false, 1.0f);
//			return;
//		}
//
//		if (m_target.lock()->GetIsAnimator())
//		{
//			m_target.lock()->GetConText()->Idol();
//			return;
//		}
//
//		if (m_KeyType & Player_ActionConText::KeyType::GuardKey)
//		{
//			m_flow = Flow::StartType;
//			return;
//		}
//	}
//}
//
//void Player_Guard::GuardRotate(Math::Vector3 _pos)
//{
//	//今の方向
//	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetAngle().y));
//	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(m_target.lock()->GetForward().x, m_target.lock()->GetForward().y, m_target.lock()->GetForward().z), nowRot);
//
//	//向きたい方向
//	Math::Vector3 toVec = _pos - m_target.lock()->GetPos();
//	toVec.Normalize();
//
//	//内角 回転する角を求める
//	float d = nowVec.Dot(toVec);
//	d = std::clamp(d, -1.0f, 1.0f); //誤差修正
//
//	//回転角度を求める
//	float ang = DirectX::XMConvertToDegrees(acos(d));
//
//	//角度変更
//	//外角　どっち回転かを求める
//	Math::Vector3 c = toVec.Cross(nowVec);
//	Math::Vector3 angle = m_target.lock()->GetAngle();
//	if (c.y >= 0)
//	{
//		//右回転
//		angle.y -= ang;
//		if (angle.y < 0.0f)
//		{
//			angle.y += 360.0f;
//		}
//
//		m_target.lock()->SetAngle(angle);
//	}
//	else
//	{
//		//左回転
//		angle.y += ang;
//		if (angle.y >= 360.0f)
//		{
//			angle.y -= 360.0f;
//		}
//
//		m_target.lock()->SetAngle(angle);
//	}
//}
//
//void Player_Guard::Hit(int _damage, std::shared_ptr<EnemyBase> _enemy)
//{
//	if (m_target.expired())return;
//
//	GuardRotate(_enemy->GetPos());
//
//	if (m_guardTime <= 30)
//	{
//		m_ObjectManager.lock()->SlowChange();
//		m_target.lock()->GetConText()->Parry(_enemy);
//	}
//	else 
//	{
//		int stamina = m_target.lock()->GetParam().Sm;
//		 stamina -= _damage;
//		if (stamina < 0)
//		{
//			stamina = 0;
//			m_target.lock()->SetStamina(stamina);
//			Player_ActionState::Hit(_damage, _enemy);
//		}
//		else
//		{
//			m_target.lock()->GetConText()->GuardReaction();
//		}
//	}
//}
//
//void Player_Guard::Hit(int _damage, std::shared_ptr<BulletBase> _bullet)
//{
//	if (m_target.expired())return;
//
//	if (m_guardTime <= 30)
//	{
//		m_ObjectManager.lock()->SlowChange();
//		m_target.lock()->GetConText()->Parry(_bullet);
//	}
//	else
//	{
//		int stamina = m_target.lock()->GetParam().Sm;
//		stamina -= _damage;
//		if (stamina < 0)
//		{
//			stamina = 0;
//			m_target.lock()->SetStamina(stamina);
//			Player_ActionState::Hit(_damage, _bullet);
//		}
//		else
//		{
//			m_target.lock()->GetConText()->GuardReaction();
//			_bullet->SetCrush(true);
//		}
//	}
//}
