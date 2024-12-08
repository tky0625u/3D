#pragma once

class SkyBox :public KdGameObject
{
public:
	SkyBox()                  { Init(); }
	~SkyBox()        override {};

	void Update()    override;
	void DrawUnLit() override;
	void Init()      override;

	void SetTarget(std::shared_ptr<KdGameObject> _target) { m_target = _target; }

private:
	
	std::weak_ptr<KdGameObject> m_target;

	std::shared_ptr<KdModelWork> m_model = nullptr;
};