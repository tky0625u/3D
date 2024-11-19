#pragma once
#include"../EnemyBase.h"

class Golem :public EnemyBase
{
public:
	Golem() {};
	~Golem()override {};

	void DrawDebug()override;
	void Init()     override;

	const Math::Matrix GetLaserPoint()const { return m_model->FindWorkNode("LaserPoint")->m_worldTransform * m_mWorld; }

private:

};