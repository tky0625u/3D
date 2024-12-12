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
	void ClearTraject();
	void SetTrajectMat();
	void SetTrajectPointNUM(int _pointNUM) { m_trajePointNUM = _pointNUM; }
	void SetATK(int _atk) { m_atk = _atk; }

	Math::Matrix GetModelTop()    { return m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	Math::Matrix GetModelCenter() { return m_spModel->FindWorkNode("sword.Center")->m_localTransform * m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	Math::Matrix GetModelBottom() { return m_spModel->FindWorkNode("sword.Bottom")->m_localTransform * m_spModel->FindWorkNode("sword.Center")->m_worldTransform * m_mWorld; }
	const bool GetTrajectPolygon()const 
	{
		if (m_traject.m_trajectPolyGon)return true;

		return false;
	}
	const int GetATK()const { return m_atk; }
	// デバッグ
	const int GetTraject()const { return m_trajePointNUM; }

private:
	struct Traject
	{
		std::shared_ptr<KdTrailPolygon> m_trajectPolyGon = nullptr;
		Math::Matrix                    m_trajectMatList[5];
	};

	Math::Vector3                     m_beforeModelTopPos = Math::Vector3::Zero;
	Traject                           m_traject;
	std::shared_ptr<KdTexture>		  m_trajeTex      = nullptr;
	int								  m_trajePointNUM = 0;
	int								  m_atk           = 0;
};