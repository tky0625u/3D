#include "CharacterBase.h"
#include"../../Scene/SceneManager.h"

void CharacterBase::Update()
{
	if (m_status.HP <= 0)
	{
		CrushingAction();
	}
	else
	{
		Action();
	}

	m_gravity += m_gravityPow;
	m_pos.y -= m_gravity;

	//ワールド行列更新
	Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = RotY * Trans;
}

void CharacterBase::PostUpdate()
{
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	float LitleUP = 0.1f;
	rayInfo.m_pos.y -= m_sizeHalf - LitleUP;
	rayInfo.m_dir = Math::Vector3::Down;
	rayInfo.m_range = m_gravity;
	rayInfo.m_type = KdCollider::TypeGround;

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
		m_pos = _hitPos;
		m_pos.y += m_sizeHalf;
		m_gravity = 0.0f;
	}
}

void CharacterBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::Init()
{
	m_model = std::make_shared<KdModelData>();

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void CharacterBase::CrushingAction()
{
	if(!IsExpired())m_isExpired = true;
}

void CharacterBase::StatusLoad(std::string a_filePath)
{
	std::ifstream ifs(a_filePath);

	if (!ifs.is_open())return;

	std::string lineString;
	std::vector<int> comma; //読み取ったステータスを格納する配列
	while (getline(ifs, lineString))
	{
		std::istringstream iss(lineString);
		std::string commaString;

		while (getline(iss, commaString, ','))
		{
			getline(iss, commaString, ',');  //最初は名称が書かれているので飛ばす
			comma.push_back(stoi(commaString));
		}
	}

	//ステータス更新
	m_status = { comma[StatusType::HPType],comma[StatusType::ATKType],comma[StatusType::MPType],comma[StatusType::DFType],comma[StatusType::SPType],comma[StatusType::SMType] };

	ifs.close();
}
