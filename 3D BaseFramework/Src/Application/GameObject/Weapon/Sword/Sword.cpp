#include "Sword.h"
#include"../../Character/CharacterBase.h"

void Sword::Update()
{
	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();

	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _targetMat = _target->GetSwordMat();
	m_mWorld = _trans * _targetMat;
}

void Sword::PostUpdate()
{

}
