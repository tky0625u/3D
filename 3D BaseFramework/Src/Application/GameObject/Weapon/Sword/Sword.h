#pragma once
#include"../WeaponBase.h"

class Sword :public WeaponBase
{
public:
	Sword()                   {}
	~Sword()         override {};

	void Update()    override;
	void PostUpdate()override;
	void DrawUnLit() override;
	void Init()      override;

	void MakeTraject();
	void SetTrajectMat();

	Math::Matrix GetModelTop()    { return m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	Math::Matrix GetModelCenter() { return m_spModel->FindWorkNode("sword.Center")->m_localTransform * m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	Math::Matrix GetModelBottom() { return m_spModel->FindWorkNode("sword.Bottom")->m_localTransform * m_spModel->FindWorkNode("sword.Center")->m_worldTransform * m_mWorld; }

private:
	struct Traject
	{
		std::shared_ptr<KdTrailPolygon> m_trajectPolyGon = nullptr;
		Math::Matrix                    m_trajectMat;
	};

	std::vector<Traject>        m_trajectList;
	std::shared_ptr<KdTexture>  m_trajeTex  = nullptr;
};