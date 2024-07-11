#include "CharacterBase.h"

void CharacterBase::Update()
{
	//ワールド行列更新
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos); //座標
	m_mWorld = Trans;  //ワールド行列
}

void CharacterBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::Init()
{
	m_model = std::make_shared<KdModelData>();
}

void CharacterBase::StatusLoad(std::string a_filePath)
{
	
}
