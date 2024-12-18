#include "CharacterBase.h"
#include"../../Scene/SceneManager.h"
#include"../ObjectManager.h"
#include"Enemy/EnemyBase.h"
#include"Player/Player.h"
#include"../Camera/GameCamera/GameCamera.h"

void CharacterBase::PreUpdate()
{
	// 移動方向リセット
	m_dir.x = 0.0f;
	m_dir.z = 0.0f;
}

void CharacterBase::Update()
{
	if (m_camera.lock()->GetShakeFlg())return;
	// デバッグ
	if (!SceneManager::Instance().m_stop)
	{
		// 行動
		Action();

		// スロー値
		float _slow = 1.0f;

		_slow = m_ObjectManager.lock()->GetSlow();

		// 重力       重力加速度   スロー値
		m_gravity += m_gravityPow * _slow;

		//   移動量   移動量           素早さ補正     スロー値
		float Move = m_MoveSpeed * m_SpeedCorrection * _slow;

		m_pos   += Move * m_dir; //座標更新
		
		//            ジャンプ力　　　  重力
		m_pos.y += m_param.JumpPow - m_gravity;
	}

	//ワールド行列更新
	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);                                    // 拡縮
	Math::Matrix RotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y)); // 回転
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);                               // 座標
	// 行列合成
	m_mWorld = Scale * RotY * Trans;
}

void CharacterBase::PostUpdate()
{
	if (m_camera.lock()->GetShakeFlg())return;

	// 地面当たり判定
	GroundCheck();

	// 横方向当たり判定
	BumpCheck();

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
	// ディゾルブ
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dissolve);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::DrawLit()
{
	// ディゾルブ
	Math::Vector3 _color = { 1,0.5f,0 };
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dissolve, nullptr, &_color);

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
	if (m_dissolve < 1.0f)m_dissolve += 0.01f; // ディゾルブ加算
	if (m_dissolve >= 1.0f)
	{
		m_dissolve = 1.0f;
	}
}

void CharacterBase::GroundCheck()
{
	// レイ判定
	KdCollider::RayInfo rayInfo;

	// 開始位置
	rayInfo.m_pos = m_pos;
	float LitleUP = 1.0f;
	rayInfo.m_pos.y += LitleUP; // 埋まらないため少し上げる
	
	// 方向
	rayInfo.m_dir = Math::Vector3::Down;
	
	// 長さ
	rayInfo.m_range = m_gravity + LitleUP;
	
	// 対象
	rayInfo.m_type = KdCollider::TypeGround;


	//デバッグ用
	//Math::Color color = { 1,1,1,1 };
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, color);

	std::list<KdCollider::CollisionResult> retRayList; // 当たり判定リスト
	for (auto& ret : SceneManager::Instance().GetObjList()) // キャラクター以外のオブジェクト判定
	{
		ret->Intersects(rayInfo, &retRayList);
	}


	// 当たり判定選出
	float _maxOverLap = 0.0f;                    // はみ出し距離
	Math::Vector3 _hitPos = Math::Vector3::Zero; // 当たった座標
	bool _hitFlg = false;                        // 当たり判定フラグ
	for (auto& ray : retRayList)
	{
		if (_maxOverLap < ray.m_overlapDistance) // はみ出し距離が長い＝一番近いオブジェクト
		{
			// 各値更新
			_maxOverLap = ray.m_overlapDistance;
			_hitPos = ray.m_hitPos;
			_hitFlg = true;
		}
	}

	// 押し出し
	if (_hitFlg)
	{
		m_pos = _hitPos;
		m_dir.y = 0.0f;
		m_gravity = 0.0f;
		m_param.JumpPow = 0.0f;
	}
}

void CharacterBase::BumpCheck()
{
	// スフィア判定
	KdCollider::SphereInfo sphereInfo;
	
	// 中心位置
	Math::Matrix _mat = m_model->FindWorkNode("spine")->m_worldTransform * (Math::Matrix::CreateTranslation(m_mWorld.Translation()));
	sphereInfo.m_sphere.Center = _mat.Translation();
	
	// 半径
	sphereInfo.m_sphere.Radius = m_HitSphereSize;
	
	// 対象
	sphereInfo.m_type = KdCollider::TypeBump;

	//デバッグ用
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, Math::Color{ 0,1,1,1 });

	std::list<KdCollider::CollisionResult>retSphereList; // 当たり判定リスト
	for (auto& ret : SceneManager::Instance().GetObjList()) // キャラクター以外のオブジェクト
	{
		ret->Intersects(sphereInfo, &retSphereList);
	}
	for (auto& ret : SceneManager::Instance().GetEnemyList()) // 敵
	{
		if (ret->IsExpired())continue; // 存在していなかったら
		if (m_id == ret->GetID())continue; // 自分だったら　同じID ＝ 自分
		ret->Intersects(sphereInfo, &retSphereList);
	}
	if (m_id != SceneManager::Instance().GetPlayer()->GetID())SceneManager::Instance().GetPlayer()->Intersects(sphereInfo, &retSphereList); // プレイヤー


	// 当たり判定選出
	float maxOverLap = 0.0f;                    // はみ出し距離
	Math::Vector3 HitDir = Math::Vector3::Zero; // 当たった方向
	bool HitFlg = false;                        // 当たり判定フラグ
	for (auto& sphere : retSphereList)
	{
		if (maxOverLap < sphere.m_overlapDistance) // はみ出し距離が長い＝一番近いオブジェクト
		{
			// 各値更新
			maxOverLap = sphere.m_overlapDistance;
			HitDir = sphere.m_hitDir;
			HitFlg = true;
		}
	}

	// 押し出し
	if (HitFlg == true)
	{
		HitDir.y = 0.0f;
		HitDir.Normalize();
		m_pos += maxOverLap * HitDir;
	}
}

void CharacterBase::Rotate(Math::Vector3 _moveDir, float _angChange)
{
	//今の方向
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(m_forward, nowRot);

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
		if (ang > _angChange)
		{
			ang = _angChange; //変更角度
		}

		//外角　どっち回転かを求める
		Math::Vector3 c = toVec.Cross(nowVec);
		if (c.y >= 0)
		{
			//右回転
			m_angle.y -= ang;
			if (m_angle.y < 0.0f)
			{
				m_angle.y += 360.0f;
			}
		}
		else
		{
			//左回転
			m_angle.y += ang;
			if (m_angle.y >= 360.0f)
			{
				m_angle.y -= 360.0f;
			}
		}
	}
}

void CharacterBase::CameraTransform(Math::Vector3& _dir)
{
	// カメラ回転行列
	Math::Matrix cameraRotYMat = Math::Matrix::Identity;
	if (m_camera.expired() == false)
	{
		cameraRotYMat = m_camera.lock()->GetRotationYMatrix();
	}
	_dir = _dir.TransformNormal(_dir, cameraRotYMat); //方向修正

	_dir.Normalize(); //正規化
}

void CharacterBase::SetParam(int _hp, int _atk, float _speed, int _stamina)
{
	// HP
	m_param.Hp = _hp;
	
	// 攻撃力
	m_param.Atk = _atk;
	
	// 素早さ
	m_param.Sp = _speed;
	
	// スタミナ
	m_param.Sm = _stamina;
}