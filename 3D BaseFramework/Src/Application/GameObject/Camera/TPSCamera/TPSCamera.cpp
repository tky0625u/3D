#include "TPSCamera.h"
#include"../../../Scene/SceneManager.h"
#include"../../ObjectManager.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	// 注視点
	m_mLocalPos = Math::Matrix::CreateTranslation(m_targetPos);

	//SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
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
			UpdateRotateByMouse();
			//ShowCursor(false);
		}
		else
		{
			ShowCursor(true);

		}

		m_mRotation = GetRotationMatrix();
		Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
		m_mWorld = m_mRotation * Trans;
		CameraBase::PostUpdate();
		return;
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

	// デバッグ
	if (!(GetAsyncKeyState(VK_LSHIFT) & 0x8000))
	{
		UpdateRotateByMouse();
		//ShowCursor(false);
	}
	else
	{
		ShowCursor(true);
	}
	
	m_mRotation = GetRotationMatrix();

	Math::Matrix _trans;
	if (!m_fixFlg)_trans = m_mLocalPos;
	else
	{
		Math::Vector3 _pos = m_fixPos;
		_trans = Math::Matrix::CreateTranslation(_pos);
		_targetMat = m_FixedTargetList[ObjectManager::Instance().GetnowStage() - 1].lock()->GetMatrix();
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
