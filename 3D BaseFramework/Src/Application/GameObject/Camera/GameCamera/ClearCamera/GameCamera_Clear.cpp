#include "GameCamera_Clear.h"
#include"../GameCamera.h"
#include"../../../Character/Player/Player.h"

void ClearCamera::Update()
{
	m_angle = m_target.lock()->GetNowDegAng();
	m_angle.y += 0.05f;
	if (m_angle.y >= 360.0f)m_angle.y -= 360.0f;
	m_target.lock()->SetDegAng(m_angle);
}

void ClearCamera::PostUpdate()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	const std::shared_ptr<const KdGameObject>	_spTarget = m_target.lock()->GetwpTarget().lock();
	if (_spTarget)
	{
		Math::Matrix _rotY = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_spTarget->GetAngle().y + 180.0f));
		Math::Matrix _trans = Math::Matrix::CreateTranslation(_spTarget->GetPos());
		_targetMat = _rotY * _trans;
	}

	m_mRotation = m_target.lock()->GetRotationMatrix();
	m_mLocalPos = Math::Matrix::CreateTranslation(m_target.lock()->GetNowPos());

	m_mWorld = m_mLocalPos * m_mRotation * _targetMat;
	m_target.lock()->SetMatrix(m_mWorld);
}
