#include "Enemy_Run.h"
#include"../../../Enemy/EnemyBase.h"
#include"../../../../../Scene/SceneManager.h"
#include"../Enemy_ConText.h"

#include"../../../Player/Player.h"
#include"../Idol/Enemy_Idol.h"
#include"../Attack/Enemy_Attack.h"
#include"../Hit/Enemy_Hit.h"
#include"../Stumble/Enemy_Stumble.h"

void Enemy_Run::Init()
{
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Enemy_Run::Start()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "IdolToRun")
		{
			m_target.lock()->SetAnime("IdolToRun", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			m_flow = Flow::CenterType;
			return;
		}

		Event();
	}
}

void Enemy_Run::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Run")
		{
			m_target.lock()->SetAnime("Run", true, 0.7f);
			return;
		}

		Event();
	}
}

void Enemy_Run::End()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "RunToIdol")
		{
			m_target.lock()->SetAnime("RunToIdol", false, 1.5f);
			return;
		}

		if (m_target.lock()->GetIsAnimator())
		{
			if (m_target.expired() == false)
			{
				if (!m_atkFlg)Idol(m_conText);
				else{Attack(m_conText);}
			}
			return;
		}
	}
}

void Enemy_Run::Event()
{
	if (!ChaseCheck())
	{
		m_flow = Flow::EndType;
		return;
	}
	AttackCheck();

	if (m_player.expired())return;
	std::shared_ptr<Player> _player = m_player.lock();
	Math::Vector3 _playerPos = _player->GetPos();
	Math::Vector3 _pos = m_target.lock()->GetPos();
	Math::Vector3 _moveDir = _playerPos - _pos;
	float dist = _moveDir.Length();
	_moveDir.Normalize();

	if (m_target.expired() == false)Rotate(_moveDir, m_target.lock());
	if(dist>=m_target.lock()->GetParam().AtkRange)m_target.lock()->SetMove(_moveDir);
}

bool Enemy_Run::AttackCheck()
{
	KdCollider::SphereInfo sphereInfo;
	Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ m_target.lock()->GetParam().ForwardX,m_target.lock()->GetParam().ForwardY,m_target.lock()->GetParam().ForwardZ }, nowRotY);
	nowVec.Normalize();
	sphereInfo.m_sphere.Center = m_target.lock()->GetPos() + nowVec * (m_target.lock()->GetParam().AtkRange / 2.0f);
	sphereInfo.m_sphere.Radius = m_target.lock()->GetParam().AtkRange / 2.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->GetObjType() == ObjType::oPlayer)
		{
			if (ret->Intersects(sphereInfo, &retSphereList))
			{
				m_flow = Flow::EndType;
				m_atkFlg = true;
				return m_atkFlg;
			}
		}
	}

	m_atkFlg = false;
	return m_atkFlg;
}

void Enemy_Run::Idol(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	if (m_target.expired())return;
	idol->SetTarget(m_target.lock());
	idol->SetConText(context);
	context->SetState(idol);
}

void Enemy_Run::Attack(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Attack> attack = std::make_shared<Enemy_Attack>();
	if (m_target.expired())return;
	attack->SetTarget(m_target.lock());
	attack->SetConText(context);
	context->SetState(attack);
}

void Enemy_Run::Hit(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	if (m_target.expired())return;
	hit->SetTarget(m_target.lock());
	hit->SetConText(context);
	context->SetState(hit);
}

void Enemy_Run::Stumble(std::shared_ptr<Enemy_ConText> context)
{
	std::shared_ptr<Enemy_Stumble> stumble = std::make_shared<Enemy_Stumble>();
	if (m_target.expired())return;
	stumble->SetTarget(m_target.lock());
	stumble->SetConText(context);
	context->SetState(stumble);
}
