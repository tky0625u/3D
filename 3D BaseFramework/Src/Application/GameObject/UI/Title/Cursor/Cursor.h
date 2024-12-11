#pragma once
#include"../../UIBase.h"

class Particle;

class Cursor :public UIBase
{
public:
	Cursor() {};
	~Cursor()override {};

	void Update()    override;
	void PostUpdate()override;
	void DrawSprite()override;
	void Init()      override;

	void ParticleDelete();

private:
	std::vector<std::shared_ptr<Particle>> m_ParticleList;
	static const int                       m_ParticleNum = 10;
	bool                                   m_ClickFlg    = false;
};