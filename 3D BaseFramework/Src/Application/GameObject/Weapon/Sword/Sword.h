#pragma once

// 武器基底
#include"../WeaponBase.h"

class Sword :public WeaponBase
{
public:
	Sword()                   {}
	~Sword()         override {};

	void Update()    override;
	void PostUpdate()override;
	void DrawUnLit() override;
	void Init()      override;

	// 軌跡生成
	void MakeTraject();
	// 軌跡削除
	void ClearTraject();

	// セッター============================================================================================================================
	void SetATK             (int _atk)      { m_atk = _atk; }                // 攻撃力
	void SetAttackSphereSize(float _size)   { m_AttackSphereSize = _size; }  // 攻撃判定用スフィアサイズ
	void SetTrajectMat      ();                                              // 軌跡の行列
	void SetTrajectPointNUM (int _pointNUM) { m_trajePointNUM = _pointNUM; } // 軌跡の座標の数
	//=====================================================================================================================================

	// ゲッター============================================================================================================================
	// 剣先
	Math::Matrix GetModelTop()    { return m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	// 中心
	Math::Matrix GetModelCenter() { return m_spModel->FindWorkNode("sword.Center")->m_localTransform * m_spModel->FindWorkNode("sword.Top")->m_worldTransform * m_mWorld; }
	// 持ち手
	Math::Matrix GetModelBottom() { return m_spModel->FindWorkNode("sword.Bottom")->m_localTransform * m_spModel->FindWorkNode("sword.Center")->m_worldTransform * m_mWorld; }
	// 攻撃力
	const int&   GetATK()const { return m_atk; }
	// 攻撃判定用スフィアサイズ
	const float& GetAttackSphereSize()const { return m_AttackSphereSize; }
	// 軌跡
	const bool   GetTrajectPolygon()const 
	{
		if (m_traject.m_trajectPolyGon)return true;

		return false;
	}
	// デバッグ　軌跡の座標の数
	const int& GetTraject()const { return m_trajePointNUM; }
	//=====================================================================================================================================

private:
	int								  m_atk               = 0;                   // 攻撃力
	float                             m_AttackSphereSize  = 0.0f;                // 攻撃判定用スフィアサイズ
	Math::Vector3                     m_beforeModelTopPos = Math::Vector3::Zero; // 前回の剣先座標

	// 軌跡行列の列挙型
	enum TrajectType
	{
		StartType,
		TransFormType,
		EndType,
		Num
	};

	// 軌跡構造体
	struct Traject
	{
		// ポリゴン
		std::shared_ptr<KdTrailPolygon> m_trajectPolyGon = nullptr;
		// 行列リスト
		std::vector<Math::Matrix>       m_trajectMatList;
	};
	
	// 軌跡構造体
	Traject                           m_traject;
	// テクスチャ
	std::shared_ptr<KdTexture>		  m_trajeTex          = nullptr;
	// 軌跡の座標の数
	int								  m_trajePointNUM     = 0;
};