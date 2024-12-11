#include "Sword.h"
#include"../../Character/CharacterBase.h"

void Sword::Update()
{	
	if (m_traject.m_trajectPolyGon != nullptr)
	{
		for (auto& _mat : m_traject.m_trajectMatList)
		{
			if (_mat == Math::Matrix::Identity)break;

			m_traject.m_trajectPolyGon->AddPoint(_mat);
		}
	}

	WeaponBase::Update();

	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix _targetMat = _target->GetSwordMat();
	m_mWorld = _scale * _trans * _targetMat;
}

void Sword::PostUpdate()
{

}

void Sword::DrawUnLit()
{
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Add);
	Math::Color _color = { 1.0f,1.0f,1.0f,0.5f };
	if (m_traject.m_trajectPolyGon != nullptr)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_traject.m_trajectPolyGon, Math::Matrix::Identity, _color);
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Alpha);
}

void Sword::Init()
{
	WeaponBase::Init();

	m_trajeTex = std::make_shared<KdTexture>();
	m_trajeTex->Load("Asset/Textures/Weapon/Trajectory/SwordLine01.png");

	for (auto& _mat : m_traject.m_trajectMatList)_mat = Math::Matrix::Identity;
}

void Sword::MakeTraject()
{
	Traject _traject;

	std::shared_ptr<KdTrailPolygon> _trajePoly = std::make_shared<KdTrailPolygon>();
	_trajePoly->SetMaterial(m_trajeTex);
	_trajePoly->SetLength(m_trajePointNUM);
	_traject.m_trajectPolyGon = _trajePoly;
	m_traject = _traject;
	m_beforeModelTopPos = GetModelTop().Translation();
}

void Sword::ClearTraject()
{
	m_traject.m_trajectPolyGon = nullptr;
	for (auto& _mat : m_traject.m_trajectMatList)_mat = Math::Matrix::Identity;
}

void Sword::SetTrajectMat()
{
	if (m_beforeModelTopPos == GetModelTop().Translation())return;
	
	Math::Vector3 _ModelTopPos = GetModelTop().Translation();
	Math::Vector3 _BM = (1.0f - 0.5f) * m_beforeModelTopPos + 0.5f * _ModelTopPos;
	Math::Vector3 _targetDir = m_target.lock()->GetPos() - _BM;
	_targetDir *= -1.0f;
	_targetDir.Normalize();
	Math::Vector3 _CenterPos = _BM + (0.25f * _targetDir);
	Math::Vector3 _BC = (1.0f - 0.5f) * m_beforeModelTopPos + 0.5f * _CenterPos;
	Math::Vector3 _CM = (1.0f - 0.5f) * _CenterPos + 0.5f * _ModelTopPos;
	Math::Vector3 _pos = (1.0f - 0.5f) * _BC + 0.5f * _CM;

	for (int i=0;i<3;++i)
	{
		Math::Matrix _trans;
		if (i == 0)_trans = Math::Matrix::CreateTranslation(m_beforeModelTopPos);
		else if (i == 1)_trans = Math::Matrix::CreateTranslation(_pos);
		else { _trans = Math::Matrix::CreateTranslation(_ModelTopPos); }

		m_traject.m_trajectMatList[i] = _trans;
	}

	m_beforeModelTopPos = _ModelTopPos;
}
