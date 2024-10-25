#include "Loading.h"

void Loading::Update()
{
	m_angle++;
	if (m_angle > 360.0f)m_angle -= 360.0f;

	m_pos.x++;
	if (m_pos.x > 650.0f)m_pos.x = -650.0f;

	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _rot = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	Math::Matrix _trans = Math::Matrix::CreateTranslation(Math::Vector3{ m_pos.x,m_pos.y,0.0f });
	m_mWorld = _scale * _rot * _trans;
}

void Loading::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width, (int)m_rect.height, &m_rect, &m_color);
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void Loading::Init()
{
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/Loading/Player.png");
	m_rect = { 0,0,24,24 };
	m_alpha = 1.0f;
	m_color = { 1,1,1,m_alpha };
	m_size = 1.0f;
}
