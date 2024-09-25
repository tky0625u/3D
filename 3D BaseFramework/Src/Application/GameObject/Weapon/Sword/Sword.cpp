#include "Sword.h"
#include"../../Character/CharacterBase.h"

void Sword::Update()
{
	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();
	
	for (int t1 = 0; t1 < m_trajectList.size(); ++t1)
	{
		for (int t2 = 0; t2 < m_trajectList[t1].size(); ++t2)
		{
			m_trajectList[t1][t2].m_trajectPolyGon->AddPoint(m_trajectList[t1][t2].m_trajectMat);
		}
	}

	WeaponBase::Update();

	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _targetMat = _target->GetSwordMat();
	m_mWorld = _trans * _targetMat;
}

void Sword::PostUpdate()
{
	auto traje1 = m_trajectList.begin();

	while (traje1 != m_trajectList.end())
	{
		auto traje2 = traje1->begin();
		while (traje2 != traje1->end())
		{
			if (traje2->m_trajectPolyGon->GetNumPoints() == m_trajePointNUM && traje2->m_trajectPolyGon->GetTopPoint()->Translation() == traje2->m_trajectPolyGon->GetPointsList()[traje2->m_trajectPolyGon->GetNumPoints() - 1].Translation())
			{
				traje2 = traje1->erase(traje2);
			}
			else
			{
				traje2++;
			}
		}

		if (traje1->size() == 0)
		{
			traje1 = m_trajectList.erase(traje1);
		}
		else
		{
			traje1++;
		}
	}
}

void Sword::DrawUnLit()
{
	for (int t1 = 0; t1 < m_trajectList.size(); ++t1)
	{
		for (int t2 = 0; t2 < m_trajectList[t1].size(); ++t2)
		{
			if (m_trajectList[t1][t2].m_trajectPolyGon != nullptr)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_trajectList[t1][t2].m_trajectPolyGon);
		}
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
	std::vector<Traject> _trajectList;
	_trajePoly->SetMaterial(m_trajeTex);
	_trajePoly->SetLength(m_trajePointNUM);
	_traject.m_trajectPolyGon = _trajePoly;
	_traject.m_trajectMat     = GetModelTop();
	_trajectList.push_back(_traject);
	m_trajectList.push_back(_trajectList);
}

void Sword::SetTrajectMat()
{
	if (m_trajectList.size() != 0 && m_trajectList[m_trajectList.size() - 1].size() != 0)m_trajectList[m_trajectList.size() - 1][m_trajectList[m_trajectList.size() - 1].size() - 1].m_trajectMat = GetModelTop();
}
