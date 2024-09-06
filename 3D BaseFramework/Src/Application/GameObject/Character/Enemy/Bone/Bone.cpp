#include "Bone.h"
#include"../../../../Scene/SceneManager.h"
#include"../../Player/Player.h"
#include"../../Action/Enemy/Enemy_ConText.h"
#include"../../Action/Enemy/Idol/Enemy_Idol.h"

void Bone::PreUpdate()
{
	m_state = m_conText->GetState();
}

void Bone::Action()
{
	m_dir = Math::Vector3::Zero;
	float Move = 0.0f;

	if (m_state.expired() == false)
	{
		int flow = m_state.lock()->GetFlow();

		     if (flow == m_state.lock()->Flow::StartType)  { m_state.lock()->Start(); }
		else if (flow == m_state.lock()->Flow::CenterType) { m_state.lock()->Center(); }
		else if (flow == m_state.lock()->Flow::EndType)    { m_state.lock()->End(); }
	}

	Move = m_param.Sp * m_SpeedCorrection;
	m_pos += Move * m_dir; //座標更新
}

void Bone::Init()
{
	CharacterBase::Init();

	m_model->SetModelData("Asset/Models/Character/Enemy/Bone/Bone.gltf");
	m_pos = { 0.0f,0.0f,20.0f };

	std::shared_ptr<Enemy_Idol> idol = std::make_shared<Enemy_Idol>();
	idol->SetTarget(shared_from_this());

	m_conText = std::make_shared<Enemy_ConText>(idol);
	m_conText->SetPlayer(m_player.lock());
	idol->SetConText(m_conText);
	m_state = m_conText->GetState();
	if (m_player.expired() == false)
	{
		m_conText->SetPlayer(m_player.lock());
		idol->SetPlayer(m_player.lock());
	}

	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;

	m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);

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
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, color);

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
