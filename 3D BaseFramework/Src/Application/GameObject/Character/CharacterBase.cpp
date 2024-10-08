#include "CharacterBase.h"
#include"../ObjectManager.h"
#include"../../Scene/SceneManager.h"
#include"Action/ActionBase.h"

void CharacterBase::Update()
{
	m_param.Dir = Math::Vector3::Zero;
	float Move = 0.0f;
	m_MoveSpeed = 0.0f;

	if (m_param.Hp <= 0)
	{
		CrushingAction();
	}
	else
	{
		Action();
	}

	float _slow = 1.0f;

	_slow = ObjectManager::Instance().GetSlow();
	if (m_MoveSpeed == 0.0f)
	{
		m_MoveSpeed = m_param.Sp;
	}
	m_gravity += m_gravityPow * _slow;
	m_param.Pos.y -= m_gravity;
	Move = m_MoveSpeed * m_SpeedCorrection * _slow;
	m_param.Pos += Move * m_param.Dir; //座標更新

	//ワールド行列更新
	Math::Matrix Scale = Math::Matrix::CreateScale(m_param.Size);
	Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle));
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_param.Pos);
	m_mWorld = Scale * RotY * Trans;
}

void CharacterBase::PostUpdate()
{
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_param.Pos;
	float LitleUP = 0.3f;
	rayInfo.m_pos.y += LitleUP;
	rayInfo.m_dir = Math::Vector3::Down; 
	rayInfo.m_range = m_gravity + LitleUP;
	rayInfo.m_type = KdCollider::TypeGround;

	//デバッグ用
	Math::Color color = { 1,1,1,1 };
	m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

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
		m_param.Pos = _hitPos;
		m_gravity = 0.0f;
	}
	else
	{
		m_anime = "Fall";
		m_animeFlg = true;
		m_animeSpeed = 1.0f;
	}

	if (ObjectManager::Instance().GetStopTime() > 0)return;
	KdCollider::SphereInfo sphereInfo;
	Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	sphereInfo.m_sphere.Radius = 2.0f;
	sphereInfo.m_type = KdCollider::TypeBump;

	//デバッグ用
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

	std::list<KdCollider::CollisionResult>retSphereList;
	for (auto& ret : SceneManager::Instance().GetObjList())
	{
		if (m_id != ret->GetID())
		{
			ret->Intersects(sphereInfo, &retSphereList);
		}
	}

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
		m_param.Pos += maxOverLap * HitDir;
	}

	//アニメーションの更新
	if (m_anime != m_beforeAnime)
	{
		m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_anime), m_animeFlg);
		m_beforeAnime = m_anime;
	}
	float _slow = 1.0f;
	_slow = ObjectManager::Instance().GetSlow();
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
	if (m_anime != "Death")
	{
		m_anime = "Death";
		m_animeFlg = false;
		m_animeSpeed = 1.0f;
	}

	m_dossolve += 0.01f;
}

void CharacterBase::Hit(int _damege)
{
	m_param.Hp -= _damege;
	if (m_param.Hp <= 0)m_param.Hp;
}

void CharacterBase::SetParam(int _hp, int _atk, float _speed, int _stamina, Math::Vector3 _pos, Math::Vector3 _dir, float _angle, float _size, float _atkRange, Math::Vector3 _forword)
{
	m_param.Hp = _hp;
	m_param.Atk = _atk;
	m_param.Sp = _speed;
	m_param.Sm = _stamina;
	m_param.Pos = _pos;
	m_param.Dir = _dir;
	m_param.Angle = _angle;
	m_param.Size = _size;
	m_param.AtkRange = _atkRange;
	m_param.ForwardX = _forword.x;
	m_param.ForwardY = _forword.y;
	m_param.ForwardZ = _forword.z;
}