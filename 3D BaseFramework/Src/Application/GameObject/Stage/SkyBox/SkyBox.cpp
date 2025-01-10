#include "SkyBox.h"

void SkyBox::Update()
{
	// プレイヤー行列 ※中心にプレイヤーが来るようにするため
	Math::Matrix TargetMat = Math::Matrix::Identity;
	if (m_target.expired() == false)
	{
		TargetMat = Math::Matrix::CreateTranslation(m_target.lock()->GetPos()); //プレイヤーの位置
	}

	// 行列
	// 拡縮
	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix RotY = Math::Matrix::CreateRotationY(m_angle.y);
	// 座標
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	// 行列合成
	m_mWorld = Scale * RotY * Trans * TargetMat;
}

void SkyBox::DrawUnLit()
{
	//オブジェクトを裏返す==========================================================
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullNone);

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);

	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullBack);
	//==============================================================================
}

void SkyBox::Init()
{
	// モデル生成
	m_model = std::make_shared<KdModelWork>();
	// モデル読み込み
	m_model->SetModelData("Asset/Models/Stage/SkyBox/SkyBox.gltf");

	// 行列
	// 拡縮
	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);
	// 回転
	Math::Matrix RotY = Math::Matrix::CreateRotationY(m_angle.y);
	// 座標
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	// 行列合成
	m_mWorld = Scale * RotY * Trans;
}
