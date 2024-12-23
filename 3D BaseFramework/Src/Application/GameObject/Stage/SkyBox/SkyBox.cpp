﻿#include "SkyBox.h"

void SkyBox::Update()
{
	Math::Matrix TargetMat = Math::Matrix::Identity;
	if (m_target.expired() == false)
	{
		TargetMat = Math::Matrix::CreateTranslation(m_target.lock()->GetPos()); //プレイヤーの位置
	}

	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix RotY = Math::Matrix::CreateRotationY(m_angle.y);
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
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
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData("Asset/Models/Stage/SkyBox/SkyBox.gltf");

	Math::Matrix Scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix RotY  = Math::Matrix::CreateRotationY(m_angle.y);
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Scale * RotY * Trans;
}
