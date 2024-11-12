#pragma once
#include"../MagicPolygon_State.h"

class MagicPolygon_Normal :public MagicPolygon_State
{
public:
	MagicPolygon_Normal() { m_flow = Flow::CenterType; };
	~MagicPolygon_Normal() {};

	void Center()override;

private:

};