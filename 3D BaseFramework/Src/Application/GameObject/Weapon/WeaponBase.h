#pragma once

// キャラクター基底
class CharacterBase;

class WeaponBase :public KdGameObject
{
public:
	WeaponBase()                                     {}
	~WeaponBase()                           override {};

	virtual void Update()                   override;
	virtual void GenerateDepthMapFromLight()override;
	virtual void DrawLit()                  override;
	virtual void Init()                     override;

	// セッター==============================================================================================
	// ターゲット（持ち主）
	void SetTarget(std::shared_ptr<CharacterBase> _target);
	// モデルパス
	void SetModelPath(std::string _modelPath);
	//=======================================================================================================

	// ゲッター==============================================================================================
	// 座標
	const Math::Vector3& GetPos();
	//=======================================================================================================

	// 消滅
	void Expired();

protected:
	std::weak_ptr<CharacterBase> m_target;             // ターゲット(持ち主)
	std::shared_ptr<KdModelWork> m_spModel  = nullptr; // モデル
	std::string                  m_modelPath;          // モデルパス
	float                        m_dissolve = 0.0f;    // ディゾルブ

};