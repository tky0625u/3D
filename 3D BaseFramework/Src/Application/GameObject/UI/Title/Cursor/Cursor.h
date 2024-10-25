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

	const Math::Vector2 GetVector2Pos()const { return m_pos; }

private:
	std::vector<Math::Vector2> m_PosList;
	int                        m_posNum = 0;

	enum Key
	{
		UPType = 1 << 0,
		DOWNType = 1 << 1
	};
	UINT                       m_keyFlg = 0;
};