#include "Sword.h"
#include"../../Character/CharacterBase.h"

void Sword::Update()
{
	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();
	
	for (int t = 0; t < m_trajectList.size(); ++t)
	{
		m_trajectList[t].m_trajectPolyGon->AddPoint(m_trajectList[t].m_trajectMat);
	}

	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _targetMat = _target->GetSwordMat();
	m_mWorld = _trans * _targetMat;
}

void Sword::PostUpdate()
{
	auto traje = m_trajectList.begin();

	while (traje != m_trajectList.end())
	{
		if (traje->m_trajectPolyGon->GetNumPoints() == 20 && traje->m_trajectPolyGon->GetTopPoint()->Translation() == traje->m_trajectPolyGon->GetPointsList()[traje->m_trajectPolyGon->GetNumPoints() - 1].Translation())
		{
			traje = m_trajectList.erase(traje);
		}
		else
		{
			traje++;
		}
	}
}

void Sword::DrawUnLit()
{
	for (auto& traje : m_trajectList)
	{
		if (traje.m_trajectPolyGon != nullptr)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*traje.m_trajectPolyGon);
	}
}

void Sword::Init()
{
	WeaponBase::Init();

	m_trajeTex = std::make_shared<KdTexture>();
	m_trajeTex->Load("Asset/Textures/Weapon/Trajectory/Image20240909102832.png");
}

void Sword::MakeTraject()
{
	Traject _traject;

	std::shared_ptr<KdTrailPolygon> _trajePoly = std::make_shared<KdTrailPolygon>();
	_trajePoly->SetMaterial(m_trajeTex);
	_traject.m_trajectPolyGon = _trajePoly;
	_traject.m_trajectMat = GetModelTop();
	m_trajectList.push_back(_traject);
}

void Sword::SetTrajectMat()
{
	if (m_trajectList.size() != 0)m_trajectList[m_trajectList.size() - 1].m_trajectMat = GetModelTop();
}
