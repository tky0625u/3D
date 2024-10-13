#include "Shield.h"
#include"../../Character/CharacterBase.h"

void Shield::Update()
{
	WeaponBase::Update();

	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _targetMat = _target->GetShieldMat();
	m_mWorld = _scale * _trans * _targetMat;
}