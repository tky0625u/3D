#include "Cursor.h"
#include"../../../../main.h"
void Cursor::Update()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 &&
		!(m_keyFlg & Key::UPType))
	{
		m_posNum++;
		m_angle = 180.0f;
		m_keyFlg |= Key::UPType;
		if (m_posNum == m_PosList.size())m_posNum = 0;
	}
	else if (!(GetAsyncKeyState(VK_UP) & 0x8000) &&
		      m_keyFlg & Key::UPType)
	{
		m_keyFlg ^= Key::UPType;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000 &&
		!(m_keyFlg & Key::DOWNType))
	{
		m_posNum--;
		m_angle = 180.0f;
		m_keyFlg |= Key::DOWNType;
		if (m_posNum < 0)m_posNum = m_PosList.size() - 1;
	}
	else if (!(GetAsyncKeyState(VK_DOWN) & 0x8000) &&
		      m_keyFlg & Key::DOWNType)
	{
		m_keyFlg ^= Key::DOWNType;
	}

	m_angle += 2.0f;
	if (m_angle > 360.0f)m_angle -= 360.0f;
	m_alpha = (sin(DirectX::XMConvertToRadians(m_angle))) + 1.5f;

	m_pos = m_PosList[m_posNum];
	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void Cursor::DrawSprite()
{
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Add);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Alpha);
}

void Cursor::Init()
{
	m_alpha = m_MaxAlpha;
	m_color = { 1.0f,1.0f,1.0f,m_alpha };
	m_rect = { 0,0,long(520),long(141) };
	m_size = m_MaxSize;
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Title/Cursor/Particle_Soft.png");
}
