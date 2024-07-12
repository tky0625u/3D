#include "Floor.h"

void Floor::DrawLit()
{
	ObjectBase::DrawLit();
}

void Floor::Init()
{
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;
}
