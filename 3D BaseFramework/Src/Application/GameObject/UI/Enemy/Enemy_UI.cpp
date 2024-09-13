#include "Enemy_UI.h"
#include"../../Camera/CameraBase.h"
#include"../../Character/Enemy/EnemyBase.h"

void Enemy_UI::Update()
{
	if (m_camera.expired())return;
	if (m_target.expired())return;

	m_rect = { 0,0,(200 / m_MaxHP) * m_target.lock()->GetParam().Hp,10 };

	m_camera.lock()->WorkCamera()->ConvertWorldToScreenDetail(m_target.lock()->GetMatrix().Translation(), m_pos);

	if (m_target.lock()->GetParam().Hp<=0)
	{
		int a;
	}
}

void Enemy_UI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, m_pos.x - (200 / 2), m_pos.y, &m_rect, &m_color, m_pivot);
}

void Enemy_UI::Init()
{
	m_pos = Math::Vector2::Zero;
	m_pivot = { 0.0f,0.5f };
	m_rect = { 0,0,200,10 };
	m_color = { 1,1,1,1 };
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/UI/Enemy/Enemy_HP.png");
	if (m_target.expired())return;
	m_MaxHP = m_target.lock()->GetParam().Hp;
}
