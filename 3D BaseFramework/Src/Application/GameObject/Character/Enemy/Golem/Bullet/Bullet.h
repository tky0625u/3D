#pragma once
#include"../../../BulletBase/BulletBase.h"

class BulletBase;
class Golem;

class Bullet :public BulletBase
{
public:
	Bullet()                   {};
	~Bullet()         override {};

	void Update()     override;
	void PostUpdate() override;
	void Init()       override;

	void SetGolem(std::shared_ptr<Golem> _golem) { m_golem = _golem; }

	const float& GetMaxSize()const { return m_MaxSize; }

private:
	std::weak_ptr<Golem>         m_golem;
	float                        m_MaxSize    = 3.0f;
	float                        m_sizeChange = 0.05f;
};