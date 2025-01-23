#pragma once

class Ground :public KdGameObject
{
public:
	Ground() {}
	~Ground()override {};

	void Update()override;
	void DrawLit()override;
	void Init()override;

	// セッター======================================================
	void SetMatrix(Math::Matrix _mat); // 行列
	//===============================================================

private:
	// モデル
	std::shared_ptr<KdModelWork> m_model = nullptr;
};