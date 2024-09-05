#include "Bone.h"
#include"../../../../Scene/SceneManager.h"
#include"../../Player/Player.h"
#include"../../Action/Enemy/Idol/Enemy_Idol.h"
#include"../../Action/Enemy/Run/Enemy_Run.h"
#include"../../Action/Enemy/Attack/Enemy_Attack.h"
#include"../../Action/Enemy/Stumble/Enemy_Stumble.h"
#include"../../Action/Enemy/Hit/Enemy_Hit.h"

void Bone::Action()
{
	m_dir = Math::Vector3::Zero; //ベクトルリセット
	float         Move = 0.0f;
	m_moveFlg = false;

	bool chaceFlg = false;
	if (m_NowAction != "Hit")chaceFlg = ChaceChaeck();
	float beforeAngle = m_param.Angle;
	float destans = 0.0f;
	Math::Vector3 v = Math::Vector3::Zero;

	if (m_NowAction == "Stumble")
	{
		Math::Matrix nowRotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle));
		Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3{ m_param.ForwardX,m_param.ForwardY,m_param.ForwardZ }, nowRotY);
		nowVec *= -1;
		nowVec.y = 0;
		nowVec.Normalize();

		m_pos += nowVec * 0.05f;
	}
	else if (chaceFlg)
	{
		std::shared_ptr<Player> player = nullptr;
		if (m_player.expired() == false)player = m_player.lock();
		Math::Vector3 playerPos = player->GetPos();
		v = playerPos - m_pos;
		v.y = 0.0f;
		destans = v.Length();

		Move = m_param.Sp * m_SpeedCorrection;
		if (m_param.AtkRange <= destans)
		{
			v.Normalize();
			m_pos += v * Move;
		}
		v.Normalize();

		//Rotation(v);
	}

	std::string BeforeAction = m_NowAction;
	if (m_NowAction != "Hit" && m_NowAction != "Stumble")
	{
		if (chaceFlg)
		{
			m_NowAction = "Run";
			if (m_param.AtkRange > destans && beforeAngle == m_param.Angle)
			{
				m_NowAction = "Attack";
			}
		}
		else
		{
			m_NowAction = "Idol";
		}
	}

	int flow = m_action->GetFlow();

	if (flow == m_action->StartType) { m_action->Start(); }
	else if (flow == m_action->CenterType) { m_action->Center(); }
	else if (flow == m_action->EndType) { m_action->End(); }
}

void Bone::Init()
{
	CharacterBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");
	m_pos = { 0.0f,0.0f,20.0f };


	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

	//std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	//idol->SetTarget(shared_from_this());
	//RegisterAction("Idol", idol);

	//std::shared_ptr<Enemy_Run> run = std::make_shared<Enemy_Run>();
	//run->SetTarget(shared_from_this());
	//RegisterAction("Run", run);

	//std::shared_ptr<Enemy_Attack> attack = std::make_shared<Enemy_Attack>();
	//attack->SetTarget(shared_from_this());
	//RegisterAction("Attack", attack);

	//std::shared_ptr<Enemy_Stumble> stumble = std::make_shared<Enemy_Stumble>();
	//stumble->SetTarget(shared_from_this());
	//RegisterAction("Stumble", stumble);

	//std::shared_ptr<Enemy_Hit> hit = std::make_shared<Enemy_Hit>();
	//hit->SetTarget(shared_from_this());
	//RegisterAction("Hit", hit);

	//m_action = idol;
	//m_NowAction = "Idol";

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Bone", m_model, KdCollider::TypeDamage | KdCollider::TypeBump);

	m_ObjType = ObjType::oEnemy;
}

bool Bone::ChaceChaeck()
{
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = m_pos;
	sphereInfo.m_sphere.Radius = 20.0f;
	sphereInfo.m_type = KdCollider::TypeEvent;

	Math::Color color = { 0,0,1,1 };
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, color);

	bool chaceFlg = false;

	std::list<KdCollider::CollisionResult> retSphereList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (ret->Intersects(sphereInfo, &retSphereList))
		{
			chaceFlg = true;
			return chaceFlg;
		}
		else
		{
			chaceFlg = false;
		}
	}

	return chaceFlg;
}
