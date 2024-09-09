#include "Shield.h"
#include"../../Character/CharacterBase.h"

void Shield::Update()
{
	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();

	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _targetMat = _target->GetShieldMat();
	m_mWorld = _trans * _targetMat;
}

void Shield::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Weapon/shield/Shield.gltf");
}
