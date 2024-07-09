#include "FPSCamera.h"

void FPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();
}

void FPSCamera::Update()
{
	// ターゲットの行列(有効な場合利用する)
	const std::shared_ptr<const KdGameObject>	_spTarget	= m_wpTarget.lock();	
	if (_spTarget)
	{
		m_mWorld = _spTarget->GetMatrix();
	}

	CameraBase::Update();
}
