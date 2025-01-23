#pragma once

class SkyBox :public KdGameObject
{
public:
	SkyBox()                  { Init(); }
	~SkyBox()        override {};

	void Update()    override;
	void DrawUnLit() override;
	void Init()      override;

	// セッター====================================================================================
	void SetTarget(std::shared_ptr<KdGameObject> _target); // プレイヤーターゲット
	//=============================================================================================

private:
	
	// プレイヤーターゲット
	std::weak_ptr<KdGameObject> m_target;

	// モデル
	std::shared_ptr<KdModelWork> m_model = nullptr;
};