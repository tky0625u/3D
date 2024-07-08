#include "ObjectBase.h"

void ObjectBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void ObjectBase::Init()
{
	m_model = std::make_shared<KdModelData>();
}
