#pragma once

// 地面
class Ground;

class Circle :public KdGameObject
{
public:
	Circle()               {};
	~Circle()     override {};

	void Update() override;
	void DrawLit()override;
	void Init()   override;

	// セッター====================================================================================
	void SetGround(std::shared_ptr<Ground> _target); // 地面
	void SetMatrix(Math::Matrix _mat);               // 行列
	//=============================================================================================

	// ゲッター====================================================================================
	// 魔法陣位置
	const Math::Vector3 GetMagicPolygonPoint()const;
	//=============================================================================================

private:
	// モデル
	std::shared_ptr<KdModelWork> m_spModel = nullptr;
	// 地面
	std::weak_ptr<Ground>        m_target;
};