#include "Shield.h"

// キャラクター基底
#include"../../Character/CharacterBase.h"

void Shield::Update()
{
	WeaponBase::Update();

	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();
	// 行列
	// 拡縮
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	// 座標
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	// ターゲット行列
	Math::Matrix _targetMat = _target->GetShieldMat();
	// 行列合成
	m_mWorld = _scale * _trans * _targetMat;
}