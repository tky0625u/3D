#pragma once

class Wall :public KdGameObject
{
public:
	Wall() {}
	~Wall()override {};

	void Update()override;
	void DrawLit()override;
	void Init()override;

private:
	std::shared_ptr<KdModelWork> m_model = nullptr;
};