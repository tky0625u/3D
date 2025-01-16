#include "Sword.h"

// キャラクター基底
#include"../../Character/CharacterBase.h"

void Sword::Update()
{	
	// 軌跡行列追加
	if (m_traject.m_trajectPolyGon != nullptr && m_traject.m_trajectMatList.size() == TrajectType::Num) // ポリゴンが無い&リストの数が少ない場合は追加しない
	{
		// ２個目から始める　※１個目は既に追加されているため
		for (int i = TrajectType::TransFormType; i < m_traject.m_trajectMatList.size(); ++i)
		{
			m_traject.m_trajectPolyGon->AddPoint(m_traject.m_trajectMatList[i]);
		}
	}

	WeaponBase::Update();

	// 持ち主
	std::shared_ptr<CharacterBase> _target = nullptr;
	if (m_target.expired() == false)_target = m_target.lock();
	// 行列
	// 拡縮
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	// 座標
	Math::Matrix _trans = Math::Matrix::CreateTranslation(m_pos);
	// 持ち主行列
	Math::Matrix _targetMat = _target->GetSwordMat();
	// 行列合成
	m_mWorld = _scale * _trans * _targetMat;
}

void Sword::PostUpdate()
{
	if (m_traject.m_trajectMatList.size() < TrajectType::Num)return;

	// リスト削除
	auto _traject = m_traject.m_trajectMatList.begin();

	// リストの数が１になるまで削除
	while (m_traject.m_trajectMatList.size() > TrajectType::StartType)
	{
		_traject = m_traject.m_trajectMatList.erase(_traject);
	}
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
}

void Sword::MakeTraject()
{
	// ポリゴン生成
	m_traject.m_trajectPolyGon = std::make_shared<KdTrailPolygon>();
	// テクスチャセット
	m_traject.m_trajectPolyGon->SetMaterial(m_trajeTex);
	// 軌跡の長さセット
	m_traject.m_trajectPolyGon->SetLength(m_trajePointNUM);
	// リスト追加
	m_traject.m_trajectMatList.push_back(GetModelTop());
	// 軌跡行列追加
	m_traject.m_trajectPolyGon->AddPoint(GetModelTop());
}

void Sword::ClearTraject()
{
	// ポリゴンとリストのリセット
	m_traject.m_trajectPolyGon = nullptr;
	m_traject.m_trajectMatList.clear();
}

void Sword::SetTrajectMat()
{
	// リスト追加
	m_traject.m_trajectMatList.push_back(GetModelTop());

	// リストの数が少なかったら早期リターン
	if (m_traject.m_trajectMatList.size() < TrajectType::Num)return;

	// １個目と２個目の中間点
	Math::Vector3 _ST = Math::Vector3::Zero;
	_ST = ((1.0f - 0.5f) * m_traject.m_trajectMatList[TrajectType::StartType].Translation()) + (0.5f * m_traject.m_trajectMatList[TrajectType::TransFormType].Translation());
	// ２個目と３個目の中間点
	Math::Vector3 _TE = Math::Vector3::Zero;
	_TE = ((1.0f - 0.5f) * m_traject.m_trajectMatList[TrajectType::TransFormType].Translation()) + (0.5f * m_traject.m_trajectMatList[TrajectType::EndType].Translation());
	// ２個目の補完後の座標
	Math::Vector3 _transform = Math::Vector3::Zero;
	_transform = ((1.0f - 0.5f) * _ST) + (0.5f * _TE);

	// ２個目を補完した座標に置き換える
	m_traject.m_trajectMatList[TrajectType::TransFormType].Translation(_transform);
}
