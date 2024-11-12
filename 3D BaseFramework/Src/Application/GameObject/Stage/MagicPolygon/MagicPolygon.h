#pragma once

class Circle;
class GameCamera;
class MagicPolygon_ConText;
class MagicPolygon_State;

class MagicPolygon :public KdGameObject,public std::enable_shared_from_this<MagicPolygon>
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
	void SetNextState(std::shared_ptr<MagicPolygon_State> _nextState) { m_NextState = _nextState; }
	void SetRBG(float _rgb) { m_rgb = _rgb; }

	const float& GetRGB() const { return m_rgb; }
	const std::shared_ptr<MagicPolygon_ConText>& GetConText()const { return m_conText; }

private:
	std::shared_ptr<MagicPolygon_ConText> m_conText   = nullptr;
	std::weak_ptr<MagicPolygon_State>     m_state;
	std::shared_ptr<MagicPolygon_State>   m_NextState = nullptr;

	std::weak_ptr<Circle>            m_circle;
	std::shared_ptr<KdSquarePolygon> m_spPolygon = nullptr;
	Math::Color                      m_color;
	float                            m_rgb       = 0.0f;
	bool                             m_rightFlg  = false;
};