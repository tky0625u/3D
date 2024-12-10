#pragma once
#include"../../UIBase.h"

class Select :public UIBase
{
public:
	Select() {};
	~Select()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

	void SetMaxSize(float _MaxSize) { m_MaxSize = _MaxSize; }
	void SetChangeSizeNum(float _ChangeSizeNum) { m_ChangeSizeNum = _ChangeSizeNum; }
	void SetMaxAlpha(float _MaxAlpha) { m_MaxAlpha = _MaxAlpha; }
	void SetChangeAlphaNum(float _ChangeAlphaNum) { m_ChangeAlphaNum = _ChangeAlphaNum; }

	const float GetMaxSize()const { return m_MaxSize; }
	const float GetChangeSizeNum()const { return m_ChangeSizeNum; }
	const float GetMaxAlpha()const { return m_MaxAlpha; }
	const float GetChangeAlphaNum()const { return m_ChangeAlphaNum; }


private:
	float                      m_MaxSize = 0.0f;
	float                      m_ChangeSizeNum = 1.0f;
	float                      m_MaxAlpha = 1.0f;
	float                      m_ChangeAlphaNum = 1.0f;
	float                      m_alpha = 1.0f;
	float                      m_SingleAngle = 0.0f;

};