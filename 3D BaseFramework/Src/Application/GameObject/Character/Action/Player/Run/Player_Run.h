#pragma once
#include"../../ActionBase.h"

class CharacterBase;

class Player_Run :public ActionBase
{
public:			    
	Player_Run()             { Init(); }
	~Player_Run()   override {};
				    
	void Start()    override;
	void Center()   override;
	void End()      override;

	void KeyCheck(const UINT key, const UINT before) override;
	void Move(std::shared_ptr<CharacterBase>& _player);

private:

};