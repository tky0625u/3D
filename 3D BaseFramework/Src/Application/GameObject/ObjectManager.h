#pragma once

class Player;

class ObjectManager
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	//void RegisterObj(std::shared_ptr<KdGameObject> Object, const UINT16& a_ClassId);
	void SetObjectParam(std::string _StageNUM, std::string _ObjectName,std::shared_ptr<KdGameObject> _Object);

	//template<typename T>
	//static UINT16 ClassID()
	//{
	//	static UINT16 _ID = GetNextID();

	//	return _ID;
	//}

private:

	//static const UINT16 GetNextID()
	//{
	//	static UINT16 _ID = 0;

	//	return _ID++;
	//}

	//std::unordered_map<UINT ,std::vector<std::weak_ptr<KdGameObject>>> m_ObjectMap;
};