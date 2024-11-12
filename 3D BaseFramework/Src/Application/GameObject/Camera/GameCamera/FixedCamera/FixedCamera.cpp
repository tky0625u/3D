#include"FixedCamera.h"
#include"../../../ObjectManager.h"
#include"../GameCamera.h"
#include"../../../Stage/MagicPolygon/MagicPolygon.h"
#include"../../../Stage/MagicPolygon/MagicPolygon_ConText.h"
#include"../../../Stage/MagicPolygon/MagicPolygon_State.h"

void FixedCamera::Update()
{

}

void FixedCamera::PostUpdate()
{
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	const std::shared_ptr<MagicPolygon>	_spTarget = m_target.lock()->GetFixedTarget();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetMatrix().Translation());
	}

	m_mRotation = m_target.lock()->GetRotationMatrix();
	m_mLocalPos = Math::Matrix::CreateTranslation(m_target.lock()->GetNowPos());

	m_mWorld = m_mLocalPos * m_mRotation * _targetMat;
	m_target.lock()->SetMatrix(m_mWorld);

	if (!KdEffekseerManager::GetInstance().IsPlaying(_spTarget->GetConText()->GetState()->GetHandle()))PlayerChange();
}