#include "CharacterBase.h"
#include"../../Scene/SceneManager.h"
#include"../ObjectManager.h"
#include"Enemy/EnemyBase.h"
#include"Player/Player.h"
#include"Action/ActionBase.h"
#include"../Camera/GameCamera/GameCamera.h"
#include"../Camera/GameCamera/GameCamera_ConText.h"
#include"../Camera/GameCamera/GameCamera_State.h"

void CharacterBase::PreUpdate()
{
	m_dir = Math::Vector3::Zero;
	m_MoveSpeed = 0.0f;
}

void CharacterBase::Update()
{
	if (m_camera.lock()->GetConText()->GetState()->GetShakeFlg())return;
	// デバッグ
	if (!SceneManager::Instance().m_stop)
	{
		Action();

		float _slow = 1.0f;

		_slow = m_ObjectManager.lock()->GetSlow();
		if (m_MoveSpeed == 0.0f)
		{
			m_MoveSpeed = m_param.Sp;
		}


		m_gravity += m_gravityPow * _slow;
		m_pos.y -= m_gravity;
		float Move = m_MoveSpeed * m_SpeedCorrection * _slow;
		m_pos += Move * m_dir; //座標更新
	}

	//ワールド行列更新
	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Scale * RotY * Trans;
}

void CharacterBase::PostUpdate()
{
	if (m_camera.lock()->GetConText()->GetState()->GetShakeFlg())return;

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	float LitleUP = 1.0f;
	rayInfo.m_pos.y += LitleUP;
	rayInfo.m_dir = Math::Vector3::Down; 
	rayInfo.m_range = m_gravity + LitleUP;
	rayInfo.m_type = KdCollider::TypeGround;

	//デバッグ用
	Math::Color color = { 1,1,1,1 };
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

	std::list<KdCollider::CollisionResult> retRayList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		ret->Intersects(rayInfo, &retRayList);
	}

	float _maxOverLap = 0.0f;
	Math::Vector3 _hitPos = Math::Vector3::Zero;
	bool _hitFlg = false;

	for (auto& ray : retRayList)
	{
		if (_maxOverLap < ray.m_overlapDistance)
		{
			_maxOverLap = ray.m_overlapDistance;
			_hitPos = ray.m_hitPos;
			_hitFlg = true;
		}
	}

	if (_hitFlg)
	{
		m_pos = _hitPos;
		m_gravity = 0.0f;
	}
	else
	{
		m_anime = "Fall";
		m_animeFlg = true;
		m_animeSpeed = 1.0f;
	}

	KdCollider::SphereInfo sphereInfo;
	Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	sphereInfo.m_sphere.Radius = 1.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	//デバッグ用
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

	std::list<KdCollider::CollisionResult>retSphereList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		ret->Intersects(sphereInfo, &retSphereList);
	}
	for (auto& ret : SceneManager::Instance().GetEnemyList())
	{
		if (ret->IsExpired())return;
		if (m_id == ret->GetID())continue;
		ret->Intersects(sphereInfo, &retSphereList);
	}
	if (m_id != SceneManager::Instance().GetPlayer()->GetID())SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &retSphereList);

	Math::Vector3 HitDir = Math::Vector3::Zero;
	float maxOverLap = 0.0f;
	bool HitFlg = false;

	for (auto& sphere : retSphereList)
	{
		if (maxOverLap < sphere.m_overlapDistance)
		{
			maxOverLap = sphere.m_overlapDistance;
			HitDir = sphere.m_hitDir;
			HitFlg = true;
		}
	}

	if (HitFlg == true)
	{
		HitDir.y = 0.0f;
		HitDir.Normalize();
		m_pos += maxOverLap * HitDir;
	}

	if (m_camera.lock()->GetConText()->GetState()->GetShakeFlg())return;

	//アニメーションの更新
	if (m_anime != m_beforeAnime)
	{
		m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);
		m_beforeAnime = m_anime;
	}
	float _slow = 1.0f;
	_slow = m_ObjectManager.lock()->GetSlow();
	m_animator->AdvanceTime(m_model->WorkNodes(), m_animeSpeed * _slow);
	m_model->CalcNodeMatrices();
}

void CharacterBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dossolve);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::DrawLit()
{
	Math::Vector3 _color = { 1,0.5f,0 };
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dossolve, nullptr, &_color);

	//オブジェクトを裏返す==========================================================
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullNone);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);

	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullBack);
	//==============================================================================
}

void CharacterBase::Init()
{

	m_model = std::make_shared<KdModelWork>();

	m_animator = std::make_shared<KdAnimator>();

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

}

void CharacterBase::CrushingAction()
{
	m_dossolve += 0.01f;
}

void CharacterBase::Hit(int _damege)
{
	m_param.Hp -= _damege;
	if (m_param.Hp <= 0)m_param.Hp = 0;
}

void CharacterBase::SetParam(int _hp, int _atk, float _speed, int _stamina)
{
	m_param.Hp = _hp;
	m_param.Atk = _atk;
	m_param.Sp = _speed;
	m_param.Sm = _stamina;
}