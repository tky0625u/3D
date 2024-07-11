#include "CharacterBase.h"

void CharacterBase::Update()
{
	if (m_status.HP <= 0)
	{
		CrushingAction();
	}
	else
	{
		Action();
	}

	//ワールド行列更新
	m_trans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_rot * m_trans;
}

void CharacterBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void CharacterBase::Init()
{
	m_model = std::make_shared<KdModelData>();
}

void CharacterBase::CrushingAction()
{
	if(!IsExpired())m_isExpired = true;
}

void CharacterBase::StatusLoad(std::string a_filePath)
{
	std::ifstream ifs(a_filePath);

	if (!ifs.is_open())return;

	std::string lineString;
	std::vector<int> comma; //読み取ったステータスを格納する配列
	while (getline(ifs, lineString))
	{
		std::istringstream iss(lineString);
		std::string commaString;

		while (getline(iss, commaString, ','))
		{
			getline(iss, commaString, ',');  //最初は名称が書かれているので飛ばす
			comma.push_back(stoi(commaString));
		}
	}

	//ステータス更新
	m_status = { comma[StatusType::HPType],comma[StatusType::ATKType],comma[StatusType::MPType],comma[StatusType::DFType],comma[StatusType::SPType],comma[StatusType::SMType] };

	ifs.close();
}
