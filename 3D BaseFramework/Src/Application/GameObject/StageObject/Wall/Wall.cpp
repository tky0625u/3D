#include "Wall.h"

void Wall::Init()
{
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;
}
