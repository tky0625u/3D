#include "MagicPolygon_Teleport.h"
#include"../../../../Scene/SceneManager.h"
#include"../MagicPolygon.h"
#include"../../../../main.h"
void MagicPolygon_Teleport::Start()
{
	Math::Vector3 _pos = m_target.lock()->GetMatrix().Translation();
	_pos.y += 0.1f;
	m_handle = KdEffekseerManager::GetInstance().Play("Circle/Light.efkefc", _pos, m_target.lock()->GetSize(), 1.0f, false).lock()->GetHandle();
	m_flow = Flow::CenterType;
}

void MagicPolygon_Teleport::Center()
{
	if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle))SceneManager::Instance().BlackAlphaChange(0.01f, true);
}

void MagicPolygon_Teleport::End()
{
}
