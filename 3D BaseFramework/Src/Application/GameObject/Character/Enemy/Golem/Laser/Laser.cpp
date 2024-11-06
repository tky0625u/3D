#include "Laser.h"
#include"../../../../ObjectManager.h"
#include"../../../Player/Player.h"

void Laser::Start()
{
	if (!m_effectFlg)
	{
		m_effectHandle = KdEffekseerManager::GetInstance().Play("Enemy/Golem/Laser/LaserEffectStart.efkefc", m_targetMat.Translation(), m_size, 1.0f, false).lock()->GetHandle();
		KdEffekseerManager::GetInstance().SetWorldMatrix(m_effectHandle, m_mWorld);
		m_effectFlg = true;
	}
	else if (!KdEffekseerManager::GetInstance().IsPlaying(m_effectHandle))
	{
		m_effectFlg = false;
		m_flow = Flow::CenterType;
		return;
	}
}

void Laser::Center()
{
	if (!m_effectFlg)
	{
		m_effectHandle = KdEffekseerManager::GetInstance().Play("Enemy/Golem/Laser/LaserEffect.efkefc", m_targetMat.Translation(), m_size, 1.0f, true).lock()->GetHandle();
		KdEffekseerManager::GetInstance().SetWorldMatrix(m_effectHandle, m_mWorld);
		m_laserHandle = KdEffekseerManager::GetInstance().Play("Enemy/Golem/Laser/Laser.efkefc", m_pos, m_size, 1.0f, true).lock()->GetHandle();
		KdEffekseerManager::GetInstance().SetWorldMatrix(m_laserHandle, m_mWorld);
		m_effectFlg = true;
	}
	else if (m_AliveTime <= 0)
	{
		KdEffekseerManager::GetInstance().StopEffect("Enemy/Golem/Laser/LaserEffect.efkefc");
		KdEffekseerManager::GetInstance().StopEffect("Enemy/Golem/Laser/Laser.efkefc");
		m_effectFlg = false;
		m_flow = Flow::EndType;
		return;
	}
	m_AliveTime--;
}

void Laser::End()
{
	if (!m_effectFlg)
	{
		m_effectHandle = KdEffekseerManager::GetInstance().Play("Enemy/Golem/Laser/LaserEffectFinish.efkefc", m_targetMat.Translation(), m_size, 1.0f, false).lock()->GetHandle();
		KdEffekseerManager::GetInstance().SetWorldMatrix(m_effectHandle, m_mWorld);
		m_laserHandle = KdEffekseerManager::GetInstance().Play("Enemy/Golem/Laser/LaserFinish.efkefc", m_pos, m_size, 1.0f, false).lock()->GetHandle();
		KdEffekseerManager::GetInstance().SetWorldMatrix(m_laserHandle, m_mWorld);
		m_effectFlg = true;
	}
	else if (!KdEffekseerManager::GetInstance().IsPlaying(m_effectHandle) && !KdEffekseerManager::GetInstance().IsPlaying(m_laserHandle))
	{
		Expired();
		return;
	}
}

void Laser::Update()
{
	switch (m_flow)
	{
	case Laser::StartType:
		Start();
		break;
	case Laser::CenterType:
		Center();
		break;
	case Laser::EndType:
		End();
		break;
	default:
		break;
	}

	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _pos = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = _scale * _pos * m_targetMat;
}

void Laser::PostUpdate()
{
	KdCollider::BoxInfo box;
	box.CheckBoxType(KdCollider::BoxInfo::BoxType::BoxOBB);
	box.m_Obox.Center = m_pos;
	box.m_type = KdCollider::TypeDamage;

	std::list<KdCollider::CollisionResult> _retBoxList;

	//if (m_ObjManager.lock()->GetPlayer().lock()->Intersects(box, nullptr))
	//{
	//	m_target.lock()->GetPlayer().lock()->Hit(m_damage);
	//}
}

void Laser::Init()
{
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _pos = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = _scale * _pos * m_targetMat;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}
