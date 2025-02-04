#include "UIBase.h"

void UIBase::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, int(m_rect.width * m_size), int(m_rect.height * m_size), &m_rect, &m_color, m_pivot);
}

// セッター========================================================================================
// 座標
void UIBase::SetPos(Math::Vector2 _pos)
{
	m_pos = _pos;
}
//=================================================================================================

// ゲッター========================================================================================
// 座標
const Math::Vector2& UIBase::GetVector2Pos() const
{
	return m_pos;
}
//=================================================================================================