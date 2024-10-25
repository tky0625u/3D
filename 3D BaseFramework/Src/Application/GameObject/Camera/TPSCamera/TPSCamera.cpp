#include "TPSCamera.h"
#include"../../../Scene/SceneManager.h"
#include"../../ObjectManager.h"
#include"../../Character/Enemy/EnemyBase.h"
#include"../../Character/Player/Player.h"
#include"../../Character/Action/Player/Player_ConText.h"
#include"../../Character/Action/Player/Player_ActionState.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	// 注視点
	m_mLocalPos = Math::Matrix::CreateTranslation(m_targetPos);

	//SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	ShowCursor(false);
}

void TPSCamera::Update()
{
	CameraBase::Update();

	// デバッグ
	if (SceneManager::Instance().m_stop)
	{
		Math::Vector3 dir = Math::Vector3::Zero;
		if (GetAsyncKeyState('W') & 0x8000)dir.z = 1.0f;
		if (GetAsyncKeyState('A') & 0x8000)dir.x = -1.0f;
		if (GetAsyncKeyState('S') & 0x8000)dir.z = -1.0f;
		if (GetAsyncKeyState('D') & 0x8000)dir.x = 1.0f;
		if (GetAsyncKeyState('E') & 0x8000)dir.y = 1.0f;
		if (GetAsyncKeyState('Q') & 0x8000)dir.y = -1.0f;
		dir.Normalize();
		Math::Matrix cameraRotYMat =GetRotationYMatrix();
		dir = Math::Vector3::TransformNormal(dir, cameraRotYMat);
		dir.Normalize();

		m_pos += dir * 1.0f;

		if (!(GetAsyncKeyState(VK_LSHIFT) & 0x8000))
		{
			if (!m_wpTarget.lock()->GetConText()->GetLockONFlg())
			{
				UpdateRotateByMouse();
			}
			//ShowCursor(false);
		}
		else
		{
			//ShowCursor(true);

		}

		m_mRotation = GetRotationMatrix();
		Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
		m_mWorld = m_mRotation * Trans;
		CameraBase::PostUpdate();
	}
}

void TPSCamera::PostUpdate()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	const std::shared_ptr<const KdGameObject>	_spTarget = m_wpTarget.lock();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos() );
		m_targetPos = _targetMat.Translation();
	}
	Math::Matrix _trans;

	// デバッグ
	if (!(GetAsyncKeyState(VK_LCONTROL) & 0x8000))
	{
		if (!m_wpTarget.lock()->GetConText()->GetLockONFlg())
		{
			UpdateRotateByMouse();
		}
		else
		{
			LockON();
		}
		//ShowCursor(false);
	}
	else
	{
		//ShowCursor(true);
	}

	if (!m_fixFlg)
	{

		m_mRotation = GetRotationMatrix();
		_trans = m_mLocalPos;
	}
	else
	{
		m_mRotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_fixAngle.x)) * Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_fixAngle.y));
		Math::Vector3 _pos = m_fixPos;
		_trans = Math::Matrix::CreateTranslation(_pos);
		_targetMat = m_FixedTargetList[m_ObjManager.lock()->GetnowStage() - 1].lock()->GetMatrix();
	}
	m_mWorld = _trans * m_mRotation * _targetMat;

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();
	rayInfo.m_dir = m_targetPos - GetPos();
	rayInfo.m_dir.Normalize();
	rayInfo.m_range = rayInfo.m_dir.Length();
	rayInfo.m_type = KdCollider::TypeBump;

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
		_hitPos += rayInfo.m_dir * 0.4f;
		SetPos(_hitPos);
	}

	CameraBase::PostUpdate();
}

void TPSCamera::FixedFlgChange()
{
	if (m_fixFlg)m_fixFlg = false;
	else { m_fixFlg = true; }
}

void TPSCamera::LockON()
{
	if (m_LockONTarget.expired())return;
	std::shared_ptr<EnemyBase> _target = m_LockONTarget.lock();

	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	m_DegAng.x += _mouseMove.y * 0.15f;

	// 回転制御
	m_DegAng.x = std::clamp(m_DegAng.x, -20.f, 45.f);

	Math::Vector3 _nowVec = Math::Vector3::TransformNormal(Math::Vector3{ 0,0,1 }, GetRotationYMatrix());

	Math::Vector3 _dir = _target->GetPos() - m_wpTarget.lock()->GetPos();
	_dir.y = 0.0f;
	_dir.Normalize();

	float d = _nowVec.Dot(_dir);
	d = std::clamp(d, -1.0f, 1.0f);

	float _ang = DirectX::XMConvertToDegrees(acos(d));

	//角度変更
	if (_ang >= 0.45f)
	{
		if (_ang > 10.0f)
		{
			_ang = 10.0f; //変更角度
		}

		//外角　どっち回転かを求める
		Math::Vector3 c = _dir.Cross(_nowVec);
		float angle = m_DegAng.y;
		if (c.y >= 0)
		{
			//右回転
			angle -= _ang;
			if (angle < 0.0f)
			{
				angle += 360.0f;
			}

			m_DegAng.y = angle;
		}
		else
		{
			//左回転
			angle += _ang;
			if (angle >= 360.0f)
			{
				angle -= 360.0f;
			}

			m_DegAng.y = angle;
		}
	}
}
