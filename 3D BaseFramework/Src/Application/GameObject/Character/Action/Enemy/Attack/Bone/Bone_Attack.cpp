#include "Bone_Attack.h"
#include"../../../../Enemy/EnemyBase.h"

void Bone_Attack::Center()
{
	Enemy_Attack::Center();
	Event();
}

void Bone_Attack::Event()
{
	if (m_target.expired())return;
	Math::Matrix  nowRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_target.lock()->GetParam().Angle));
	Math::Vector3 nowVec = Math::Vector3::TransformNormal(Math::Vector3(m_target.lock()->GetParam().ForwardX, m_target.lock()->GetParam().ForwardY, m_target.lock()->GetParam().ForwardZ), nowRot);
	nowVec.Normalize();

	m_target.lock()->SetMove(nowVec);
}
