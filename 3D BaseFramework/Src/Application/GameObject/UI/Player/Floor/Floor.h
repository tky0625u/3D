#pragma once
#include"../../UIBase.h"

class StageManager;

class Floor :public UIBase
{
public:
	Floor()                    {};
	~Floor()          override {};

	void Update()     override;
	void DrawSprite() override;
	void Init()       override;

	void SetStageManager(std::shared_ptr<StageManager> _stage) { m_StageManager = _stage; }

private:
	std::weak_ptr<StageManager>m_StageManager;
	std::shared_ptr<KdTexture> m_FloorPTex = nullptr;
	Math::Vector2              m_FloorPos = Math::Vector2::Zero;
	Math::Vector2              m_FloorPivot = { 0.5f,0.5f };
	Math::Rectangle            m_FloorRect;
	Math::Color                m_FloorColor = { 1,1,1,1 };
};