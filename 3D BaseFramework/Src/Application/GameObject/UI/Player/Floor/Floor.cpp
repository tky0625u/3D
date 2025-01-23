#include "Floor.h"

// シーンマネジャ
#include"../../../../Scene/SceneManager.h"
// ステージマネジャ
#include"../../../StageManager.h"

void Floor::Update()
{
	// 階層数UIの切り取り範囲変更                 現在の階層
	m_FloorRect = { 0,long(141.6f * (m_StageManager.lock()->GetnowStage() - 1)),long(95.0f),long(141.6f)};
}

void Floor::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_FloorPTex, (int)m_FloorPos.x, (int)m_FloorPos.y, (int)m_FloorRect.width * m_size, (int)m_FloorRect.height * m_size, &m_FloorRect, &m_FloorColor, m_FloorPivot);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_pTex, (int)m_pos.x, (int)m_pos.y, (int)m_rect.width * m_size, (int)m_rect.height * m_size, &m_rect, &m_color, m_pivot);
}

void Floor::Init()
{
	// Floor
	m_pivot = { 0.5f,0.5f };
	m_color = { 1,1,1,1 };
	m_rect  = { 0,0,383,121 };
	m_pTex  = std::make_shared<KdTexture>();
	m_pTex->Load("Asset/Textures/UI/Player/Floor/Cool Text - FLOOR 468658899565488.png");

	// 階層数
	m_FloorPivot = { 0.5f,0.5f };
	m_FloorColor = { 1,1,1,1 };
	m_FloorRect  = { 0,long(141.6f * (m_StageManager.lock()->GetnowStage() - 1)),long(95.0f),long(141.6f)};
	m_FloorPos   = { m_pos.x,(m_pos.y - ((m_FloorRect.height * m_FloorPivot.y * m_size) + (m_rect.height * m_pivot.y * m_size))) };
	m_FloorPTex  = std::make_shared<KdTexture>();
	m_FloorPTex->Load("Asset/Textures/UI/Player/Floor/Cool Text - 1234567890 468658586495746.png");
}

// セッター========================================================================================
// ステージマネジャ
void Floor::SetStageManager(std::shared_ptr<StageManager> _stage)
{
	m_StageManager = _stage;
}

// 座標
void Floor::SetPos(Math::Vector2 _pos)
{
	m_pos = _pos;
	m_FloorPos = { m_pos.x,(m_pos.y - ((m_FloorRect.height * m_FloorPivot.y * m_size) + (m_rect.height * m_pivot.y * m_size))) };
}
//=================================================================================================