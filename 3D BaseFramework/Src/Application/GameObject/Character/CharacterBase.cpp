#include "CharacterBase.h"
#include"../../Scene/SceneManager.h"

void CharacterBase::Update()
{
	if (m_param.Hp <= 0)
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
	Math::Matrix Scale = Math::Matrix::CreateScale(m_param.Size);
	Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle));
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Scale * RotY * Trans;
}

void CharacterBase::PostUpdate()
{
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	float LitleUP = 0.1f;
	rayInfo.m_pos.y += LitleUP;
	rayInfo.m_dir = Math::Vector3::Down;  //↓原点が少しずれているので修正
	rayInfo.m_range = m_gravity + LitleUP + m_param.ErrorNum;
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
		m_gravity = 0.0f;
	}

	//アニメーションの更新
	if (m_Action != m_beforeAction)
	{
		m_animator->SetAnimation(m_model->GetData()->GetAnimation(m_Action), m_animeFlg);
		m_beforeAction = m_Action;
	}
	m_animator->AdvanceTime(m_model->WorkNodes(), m_animeSpeed);
	m_model->CalcNodeMatrices();
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
	m_model = std::make_shared<KdModelWork>();

	m_animator = std::make_shared<KdAnimator>();

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

}

void CharacterBase::CrushingAction()
{
	if (m_Action != "Death")
	{
		m_Action = "Death";
		m_animeFlg = false;
		m_animeSpeed = 1.0f;
	}
	if(m_animator->IsAnimationEnd() && !IsExpired())m_isExpired = true;
}

void CharacterBase::Rotation(Math::Vector3 _moveDir)
{
	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_param.Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(m_param.ForwardX, m_param.ForwardY, m_param.ForwardZ), nowRot);

	//向きたい方向
	Math::Vector3 toVec = _moveDir;
	toVec.Normalize();

	//内角 回転する角を求める
	float d = nowVec.Dot(toVec);
	d = std::clamp(d, -1.0f, 1.0f); //誤差修正

	//回転角度を求める
	float ang = DirectX::XMConvertToDegrees(acos(d));

	//角度変更
	if (ang >= 0.1f)
	{
		if (ang > 20)
		{
			ang = 20.0f; //変更角度
		}

		//外角　どっち回転かを求める
		Math::Vector3 c = toVec.Cross(nowVec);
		if (c.y >= 0)
		{
			//右回転
			m_param.Angle -= ang;
			if (m_param.Angle < 0.0f)
			{
				m_param.Angle += 360.0f;
			}
		}
		else
		{
			//左回転
			m_param.Angle += ang;
			if (m_param.Angle >= 360.0f)
			{
				m_param.Angle -= 360.0f;
			}
		}
	}
}

void CharacterBase::ParamLoad(std::string a_filePath)
{
	std::ifstream ifs(a_filePath);

	if (!ifs.is_open())return;

	std::string lineString;
	std::vector<float> comma; //読み取ったパラメータを格納する配列
	while (getline(ifs, lineString))
	{
		std::istringstream iss(lineString);
		std::string commaString;

		while (getline(iss, commaString, ','))
		{
			getline(iss, commaString, ',');  //最初は名称が書かれているので飛ばす
			comma.push_back(stof(commaString));
		}
	}

	//パラメータ更新
	m_param = { (int)comma[ParamType::HpType],    //体力
				(int)comma[ParamType::AtkType],	  //攻撃力
				(int)comma[ParamType::DfType],	  //防御力
				(int)comma[ParamType::SpType],	  //素早さ
				(int)comma[ParamType::SmType],	  //スタミナ
				     comma[ParamType::AgType],	  //角度
				     comma[ParamType::SzType],	  //大きさ
				     comma[ParamType::EnType],	  //モデルの原点の誤差
				     comma[ParamType::ArType],    //攻撃範囲
				     comma[ParamType::FXType],    //攻撃範囲
				     comma[ParamType::FYType],    //攻撃範囲
				     comma[ParamType::FZType] };  //攻撃範囲

	ifs.close();
}
