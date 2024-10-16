#pragma once

class Circle :public KdGameObject
{
public:
	Circle()               {};
	~Circle()     override {};

	void Update() override;
	void DrawLit()override;
	void Init()   override;

	const Math::Vector3 GetMagicPolygonPoint()const { return (m_spModel->FindWorkNode("MagicPolygonPoint")->m_worldTransform * m_mWorld).Translation(); }

private:
	std::shared_ptr<KdModelWork> m_spModel = nullptr;
};