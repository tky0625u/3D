﻿#include "Enemy_Idol.h"
#include"../../../CharacterBase.h"

void Enemy_Idol::Center()
{
	if (m_target.expired() == false)
	{
		if (m_target.lock()->GetAnime() != "Idol")
		{
			m_target.lock()->SetAnime("Idol", true, 1.0f);
			return;
		}
	}
}

void Enemy_Idol::Reset()
{
	m_ChangeFlg = true;
	m_flow = Flow::CenterType;
	m_end = false;
}