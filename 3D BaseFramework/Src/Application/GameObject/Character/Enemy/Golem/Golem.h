#pragma once
#include"../EnemyBase.h"

class Golem :public EnemyBase,public std::enable_shared_from_this<Golem>
{
public:
	Golem() {};
	~Golem()override {};

	void DrawDebug()override;
	void Init()     override;

	const Math::Matrix GetBulletPoint()const { return m_model->FindWorkNode("BulletPoint")->m_worldTransform * m_mWorld; }
	const Math::Matrix GetQuakePoint() const { return m_model->FindWorkNode("QuakePoint")->m_worldTransform * m_mWorld; }

private:

};