#include "MagicPolygon_Normal.h"
#include"../MagicPolygon.h"

void MagicPolygon_Normal::Start()
{
	if (m_target.lock()->GetTeleport())m_target.lock()->SetTeleportFlg(false);
	m_flow = Flow::CenterType;
}

void MagicPolygon_Normal::Center()
{
	if (m_target.lock()->GetRGB() != 0.0f)
	{
		m_target.lock()->SetRBG(0.0f);
	}
}
