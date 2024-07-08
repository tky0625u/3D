#include "Floor.h"

void Floor::DrawLit()
{
	ObjectBase::DrawLit();
}

void Floor::Init()
{
	ObjectBase::Init();
	m_model->Load("Asset/Models/StageObject/Floor/Floor.gltf");
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;
}
