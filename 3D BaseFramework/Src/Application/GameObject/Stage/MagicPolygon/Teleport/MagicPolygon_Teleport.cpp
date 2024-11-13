#include "MagicPolygon_Teleport.h"
#include"../../../../Scene/SceneManager.h"
#include"../MagicPolygon.h"
#include"../../../../main.h"
void MagicPolygon_Teleport::Start()
{
	m_handle = KdEffekseerManager::GetInstance().Play("Circle/Teleport.efkefc", m_target.lock()->GetMatrix().Translation(), m_target.lock()->GetSize(), 1.0f, false).lock()->GetHandle();
	m_flow = Flow::CenterType;
	a++;
}

void MagicPolygon_Teleport::Center()
{
	if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle))SceneManager::Instance().BlackAlphaChange(0.01f, true);
	a++;
	Application::Instance().m_log.AddLog("%d\n", a);
}

void MagicPolygon_Teleport::End()
{
}
