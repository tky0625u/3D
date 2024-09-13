#pragma once

class CameraBase;
class EnemyBase;

class Enemy_UI :public KdGameObject
{
public:
	Enemy_UI ()               {}
	~Enemy_UI()       override{};

	void Update    () override;
	void DrawSprite() override;
	void Init      () override;

	void SetTarget(std::shared_ptr<EnemyBase> _target)  { m_target = _target; }
	void SetCamera(std::shared_ptr<CameraBase> _camera) { m_camera = _camera; }

private:
	std::weak_ptr<CameraBase>  m_camera;
	std::weak_ptr<EnemyBase>   m_target;
	std::shared_ptr<KdTexture> m_tex = nullptr;
	Math::Vector2              m_pos;
	Math::Vector2              m_pivot;
	Math::Rectangle            m_rect;
	Math::Color                m_color;
	int                        m_MaxHP;
};