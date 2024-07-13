#pragma once

class CharacterBase;

class ObjectManager :public KdGameObject
{
public:
	enum ObjectType
	{
		FloorType,
		WallType,
		PlayerType
	};

	ObjectManager() { Init(); }
	~ObjectManager()                  override {};

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

	const std::vector < std::shared_ptr<KdGameObject> >& GetObj()const { return m_ObjList; }

private:
	std::vector < std::shared_ptr<KdGameObject> > m_ObjList; //ステージオブジェクトリスト

	static const int                              m_ObjDistans = 4;

	std::weak_ptr<CharacterBase>                  m_player;

	std::shared_ptr<KdModelData>                  m_floorModel;
	std::shared_ptr<KdModelData>                  m_wallModel;

public:

	static ObjectManager& instance()
	{
		static ObjectManager instance;
		return instance;
	}
};