#include "UIBase.h"

void UIBase::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width, (int)m_rect.height, &m_rect, &m_color);
}
