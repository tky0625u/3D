#include "LockON.h"
#include"../../../Character/Player/Player.h"
#include"../../../Character/Enemy/EnemyBase.h"
#include"../../../Camera/TPSCamera/TPSCamera.h"
#include"../../../ObjectManager.h"

void LockON::Update()
{
	if (!m_target.lock()->GetLockONFlg())
	{
		m_alpha = 1.0f;
		m_size = 5.0f;
		return;
	}

	std::shared_ptr<EnemyBase> _targetEnemy = m_target.lock()->GetLockONTarget().lock();
	if (_targetEnemy->GetID() != m_beforeTargetID)
	{
		m_alpha = 1.0f;
		m_size = 5.0f;
		m_beforeTargetID = _targetEnemy->GetID();
	}

	if (m_size ==1.0f)
	{
		m_alpha -= 0.05f;
		if (m_alpha <= 0.0f)m_alpha = 0.0f;
	}
	else
	{
		m_size -= 0.5f;
		if (m_size <= 1.0f)m_size = 1.0f;
	}

	//座標変換
	Math::Vector3 _pos = { m_pos.x,m_pos.y,0.0f };
	ObjectManager::Instance().GetCamera().lock()->WorkCamera()->ConvertWorldToScreenDetail(_targetEnemy->GetLockPointMat().Translation(), _pos);

	m_pos = { _pos.x,_pos.y };
	m_color = { 1,1,1,m_alpha };
}

void LockON::DrawSprite()
{
	if (!m_target.lock()->GetLockONFlg())return;
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
}

void LockON::Init()
{
	m_pivot = { 0.5f,0.5f };
	m_color = { 1,1,1,m_alpha };
	m_size = 5.0f;
	m_rect = { 0,0,256,256 };
	m_pTex = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Player/LockON/Cursor_Move2.png");
}
