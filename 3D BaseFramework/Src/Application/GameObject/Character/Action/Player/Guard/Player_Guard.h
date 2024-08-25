#pragma once
#include"../../ActionBase.h"

class CharacterBase;

class Player_Guard :public ActionBase
{
public:
	Player_Guard() { Init(); }
	~Player_Guard()   override {};

	void Start()    override;
	void Center()   override;
	void End()      override;

	void KeyCheck(const UINT key, const UINT before) override;

	void Init()     override;
	void Reset()    override;

private:

};