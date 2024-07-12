#include "Floor.h"

void Floor::Init()
{
	Math::Matrix Trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = Trans;
}
