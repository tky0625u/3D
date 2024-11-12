#include "MagicPolygon_Normal.h"
#include"../MagicPolygon.h"

void MagicPolygon_Normal::Center()
{
	if (m_target.lock()->GetRGB() != 0.0f)
	{
		m_target.lock()->SetRBG(0.0f);
	}
}
