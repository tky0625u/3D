#include "Wall.h"

void Wall::DrawLit()
{
	ObjectBase::DrawLit();
}

void Wall::Init()
{
	ObjectBase::Init();
	m_model->Load("Asset/Models/StageObject/Wall/Wall.gltf");
}
