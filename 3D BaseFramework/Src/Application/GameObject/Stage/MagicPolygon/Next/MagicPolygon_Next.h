#pragma once
#include"../MagicPolygon_State.h"

class MagicPolygon_Next :public MagicPolygon_State
{
public:
	MagicPolygon_Next() {};
	~MagicPolygon_Next() {};

	void Start() override;
	void Center()override;
	void End()   override;

private:
	float m_angle = 0.0f;
	float m_rgb   = 0.0f;
};