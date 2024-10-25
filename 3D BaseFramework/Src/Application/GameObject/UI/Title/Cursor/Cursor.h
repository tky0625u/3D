#pragma once
#include"../../UIBase.h"

class Cursor :public UIBase
{
public:
	Cursor() {};
	~Cursor()override {};

	void Update()    override;
	void DrawSprite()override;
	void Init()      override;

	void SetPosList(Math::Vector2 _pos) { m_PosList.push_back(_pos); }
	void SetMaxSize(float _MaxSize) { m_MaxSize = _MaxSize; }
	void SetChangeSizeNum(float _ChangeSizeNum) { m_ChangeSizeNum = _ChangeSizeNum; }
	void SetMaxAlpha(float _MaxAlpha) { m_MaxAlpha = _MaxAlpha; }
	void SetChangeAlphaNum(float _ChangeAlphaNum) { m_ChangeAlphaNum = _ChangeAlphaNum; }

	const Math::Vector2 GetVector2Pos()const { return m_pos; }
	const float GetMaxSize()const { return m_MaxSize; }
	const float GetChangeSizeNum()const { return m_ChangeSizeNum; }
	const float GetMaxAlpha()const { return m_MaxAlpha; }
	const float GetChangeAlphaNum()const { return m_ChangeAlphaNum; }


private:
	std::vector<Math::Vector2> m_PosList;
	int                        m_posNum = 0;
	float                      m_MaxSize = 0.0f;
	float                      m_ChangeSizeNum = 1.0f;
	float                      m_MaxAlpha = 1.0f;
	float                      m_ChangeAlphaNum = 1.0f;
	float                      m_alpha = 1.0f;

	enum Key
	{
		UPType = 1 << 0,
		DOWNType = 1 << 1
	};
	UINT                       m_keyFlg = 0;
};