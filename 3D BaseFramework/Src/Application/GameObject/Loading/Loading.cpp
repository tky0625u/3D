#include "Loading.h"

void Loading::Update()
{
	m_load[0].m_angle += 0.005f;

	static float _angle = 0.0f;
	_angle += 0.025f;
	if (_angle > 360.0f)_angle -= 360.0f;
	m_load[1].m_alpha = sin(DirectX::XMConvertToRadians(_angle)) + 1.5f;
	m_load[1].m_color = { 1,1,1,m_load[1].m_alpha };

	for (auto& load : m_load)
	{
		Math::Matrix _scale = Math::Matrix::CreateScale(load.m_size);
		Math::Matrix _rot = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(load.m_angle));
		Math::Matrix _trans = Math::Matrix::CreateTranslation(Math::Vector3{ load.m_pos.x,load.m_pos.y,0.0f });
		load.m_Mat = _scale * _rot * _trans;
	}
}

void Loading::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		Math::Color _color = { 0,0,0,1 };
		KdShaderManager::Instance().m_spriteShader.DrawBox(0, 0, 640, 360, &_color, true);

		for (auto& load : m_load)
		{
			KdShaderManager::Instance().m_spriteShader.SetMatrix(load.m_Mat);
			KdShaderManager::Instance().m_spriteShader.DrawTex(load.m_tex, 0, 0, &load.m_rect, &load.m_color, load.m_pivot);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
		}
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void Loading::Init()
{
	m_load[0].m_tex   = std::make_shared<KdTexture>();
	m_load[0].m_tex->Load("Asset/Textures/Loading/magic circle red.png");
	m_load[0].m_rect  = {0,0,long(993),long(993)};
	m_load[0].m_size  = 0.1f;
	m_load[0].m_pos   = { 640.0f - 60.0f,-360.0f + 60.0f };
	m_load[0].m_alpha = 1.0f;
	m_load[0].m_color = { 1,1,1,m_load[0].m_alpha };
	m_load[0].m_pivot = { 0.5f,0.5f };

	m_load[1].m_tex   = std::make_shared<KdTexture>();
	m_load[1].m_tex->Load("Asset/Textures/Loading/NowLoading.png");
	m_load[1].m_rect  = { 0,0,long(578),long(75) };
	m_load[1].m_size  = 1.0f;
	m_load[1].m_pos   = { m_load[0].m_pos.x - ((m_load[0].m_rect.width * m_load[0].m_size) / 2.0f),-360.0f + 10.0f };
	m_load[1].m_alpha = 1.0f;
	m_load[1].m_color = { 1,1,1,m_load[1].m_alpha };
	m_load[1].m_pivot = { 1.0f,0.0f };
}
