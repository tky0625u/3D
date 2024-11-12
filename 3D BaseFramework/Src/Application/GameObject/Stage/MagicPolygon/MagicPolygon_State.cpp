#include "MagicPolygon_State.h"
#include"MagicPolygon.h"

#include"Normal/MagicPolygon_Normal.h"
#include"Next/MagicPolygon_Next.h"

void MagicPolygon_State::Normal()
{
	std::shared_ptr<MagicPolygon_Normal> _normal = std::make_shared<MagicPolygon_Normal>();
	_normal->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(_normal);
}

void MagicPolygon_State::Next()
{
	std::shared_ptr<MagicPolygon_Next> _next = std::make_shared<MagicPolygon_Next>();
	_next->SetTarget(m_target.lock());
	m_target.lock()->SetNextState(_next);
}
