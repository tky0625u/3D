#pragma once

class Circle;
class GameCamera;

class MagicPolygon :public KdGameObject
{
public:
	MagicPolygon() {};
	~MagicPolygon()override {};

	void Update()override;
	void DrawLit()override;
	void DrawBright()override;
	void Init()override;

	void SetCircle(std::shared_ptr<Circle> _circle) { m_circle = _circle; }
	void SetMatrix(Math::Matrix _mat) { m_mWorld = _mat; }
	void SetCamera(std::shared_ptr<GameCamera> _camera) { m_camera = _camera; }

private:
	std::weak_ptr<Circle>            m_circle;
	std::weak_ptr<GameCamera>        m_camera;
	std::shared_ptr<KdSquarePolygon> m_spPolygon = nullptr;
	Math::Color                      m_color;
	float                            m_rgb       = 0.0f;
	bool                             m_rightFlg  = false;
};