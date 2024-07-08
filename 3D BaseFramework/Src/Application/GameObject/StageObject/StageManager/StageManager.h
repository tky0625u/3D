#pragma once

class StageManager :public KdGameObject
{
public:
	enum ObjectType
	{
		FloorType,
		WallType
	};

	StageManager()                            { Init(); }
	~StageManager()                  override {};

	void PreUpdate()                 override;
	void Update()                    override;
	void PostUpdate()                override;

	void GenerateDepthMapFromLight() override;
	void PreDraw()                   override;
	void DrawLit()                   override;
	void DrawUnLit()                 override;
	void DrawBright()                override;
	void DrawSprite()                override;

	void Init()                      override;

	void Load(int StageNumber);

private:
	std::vector < std::shared_ptr<KdGameObject> > m_StgObjList; //ステージオブジェクトリスト

	static const int                              m_ObjDistans = 4;
};