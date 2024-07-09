#include "TPSCamera.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	// 注視点
	m_mLocalPos = Math::Matrix::CreateTranslation(0, 3.0f, -10.0f);
}

void TPSCamera::Update()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								_targetMat	= Math::Matrix::Identity;
	const std::shared_ptr<const KdGameObject>	_spTarget	= m_wpTarget.lock();
	if (_spTarget)
	{
		_targetMat = _spTarget->GetMatrix();
	}

	m_mWorld	= m_mLocalPos * _targetMat;

	CameraBase::Update();
}