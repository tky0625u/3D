#include "MagicPolygon_Next.h"
#include"../MagicPolygon.h"

void MagicPolygon_Next::Start()
{
	m_handle = KdEffekseerManager::GetInstance().Play("Circle/Circle.efkefc", m_target.lock()->GetMatrix().Translation(), m_target.lock()->GetSize(), 1.0f, false).lock()->GetHandle();
	m_angle  = m_target.lock()->GetAngle().y;
	m_rgb    = m_target.lock()->GetRGB();
	m_flow   = Flow::CenterType;
}

void MagicPolygon_Next::Center()
{
	m_angle += 0.01f;
	if (m_angle > 360.0f)m_angle -= 360.0f;
	if (m_rgb < 1.0f)    m_rgb += 0.01f;
	if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle) && !m_target.lock()->GetTeleport())m_target.lock()->SetTeleportFlg(true);

	m_target.lock()->SetAngle(Math::Vector3{ 0.0f,m_angle,0.0f });
	m_target.lock()->SetRBG(m_rgb);
}

void MagicPolygon_Next::End()
{
}
