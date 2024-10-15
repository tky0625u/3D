#pragma once

class Circle :public KdGameObject
{
public:
	Circle()               {};
	~Circle()     override {};

	void Update() override;
	void DrawLit()override;
	void Init()   override;

private:
	std::shared_ptr<KdModelWork> m_spModel = nullptr;

};