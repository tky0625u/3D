#include "Loading.h"

void Loading::Update()
{
	m_angle += 0.005f;
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _rot = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	Math::Matrix _trans = Math::Matrix::CreateTranslation(Math::Vector3{ m_pos.x,m_pos.y,0.0f });
	m_mWorld = _scale * _rot * _trans;
}

void Loading::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		Math::Color _color = { 0,0,0,1 };
		KdShaderManager::Instance().m_spriteShader.DrawBox(0, 0, 640, 360, &_color, true);

		KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width, (int)m_rect.height, &m_rect, &m_color);
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void Loading::Init()
{
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/Loading/magic circle red.png");
	m_rect = { 0,0,long(993),long(993) };
	m_size = 0.5f;
	m_pos = { 0.0f,0.0f };
	m_alpha = 1.0f;
	m_color = { 1,1,1,m_alpha };
}
