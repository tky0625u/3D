#pragma once

class Circle;

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

private:
	std::weak_ptr<Circle>            m_circle;

	std::shared_ptr<KdSquarePolygon> m_spPolygon = nullptr;
	Math::Color                      m_color;
	float                            m_rgb       = 0.0f;
	bool                             m_rightFlg  = false;
};