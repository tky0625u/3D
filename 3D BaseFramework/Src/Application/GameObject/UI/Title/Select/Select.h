#pragma once

// UI基底
#include"../../UIBase.h"

class Select :public UIBase
{
public:
	Select() {};
	~Select()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

	// セッター==============================================================================================
	void SetAlphaCorrection(float _correction); // アルファ値補正値
	void SetChangeAngle(float _change);         // 角度変化量
	//=======================================================================================================

	// ゲッター==============================================================================================
	const float& GetAlphaCorrection()const; // アルファ値補正値
	const float& GetChangeAngle()const;     // 角度変化量
	//=======================================================================================================

private:
	float m_alpha           = 1.0f;  // アルファ値
	float m_AlphaCorrection = 0.0f;  // アルファ値補正値
	float m_SingleAngle     = 0.0f;  // sinカーブ用角度
	float m_ChangeAngle     = 0.0f;  // 角度変化量
	bool  m_SelectFlg       = false; // 選択中フラグ

};