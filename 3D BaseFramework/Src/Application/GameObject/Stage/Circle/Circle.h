#pragma once

class Ground;

class Circle :public KdGameObject
{
public:
	Circle()               {};
	~Circle()     override {};

	void Update() override;
	void DrawLit()override;
	void Init()   override;

	void SetGround(std::shared_ptr<Ground> _target) { m_target = _target; }
	void SetMatrix(Math::Matrix _mat) { m_mWorld = _mat; }

	const Math::Vector3 GetMagicPolygonPoint()const { return (m_spModel->FindWorkNode("MagicPolygonPoint")->m_worldTransform * m_mWorld).Translation(); }

private:
	std::shared_ptr<KdModelWork> m_spModel = nullptr;
	std::weak_ptr<Ground>        m_target;
};