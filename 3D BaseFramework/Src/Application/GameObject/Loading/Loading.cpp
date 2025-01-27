#include "Loading.h"
#include"../tinygltf/json.hpp"
#include<fstream>
#include<sstream>

void Loading::Update()
{
	m_state->Update(shared_from_this());
}

void Loading::DrawSprite()
{
	m_state->DrawSprite(shared_from_this());
}

void Loading::Init()
{
	m_state->Init(shared_from_this());
}

void Loading::TitleChange()
{
	m_state = std::make_shared<TitleLoad>();
	m_state->SetLoadParam();
}

void Loading::GameChange()
{
	m_state = std::make_shared<GameLoad>();
	m_state->SetLoadParam();
}

// タイトル========================================================================================
void Loading::TitleLoad::SetLoadParam()
{
}

void Loading::TitleLoad::Init(std::shared_ptr<Loading> owner)
{
}

void Loading::TitleLoad::Update(std::shared_ptr<Loading> owner)
{
}

void Loading::TitleLoad::DrawSprite(std::shared_ptr<Loading> owner)
{
}
//=================================================================================================


// ゲーム==========================================================================================
void Loading::GameLoad::SetLoadParam()
{
	std::string _filePath = "Asset/Json/Game/Load/Load.json";

	std::ifstream _ifs(_filePath.c_str());
	nlohmann::json _json;
	if (_ifs.is_open())
	{
		_ifs >> _json;
	}

	for (auto& _load : _json)
	{
		// 魔法陣角度変化量
		float _CircleChangeAngle = 0.0f;
		_CircleChangeAngle = _load["CircleChangeAngle"];

		// 文字アルファ値変化量
		float _StringChangeAlpha = 0.0f;
		_StringChangeAlpha = _load["StringChangeAlpha"];

		// 文字アルファ値補正値
		float _StringCorrectAlpha = 0.0f;
		_StringCorrectAlpha = _load["StringCorrectAlpha"];
		
		// セット
		m_CircleChangeAngle = _CircleChangeAngle;
		m_StringChangeAlpha = _StringChangeAlpha;
		m_StringCorrectAlpha = _StringCorrectAlpha;
	}

	_ifs.close();
}

void Loading::GameLoad::Init(std::shared_ptr<Loading> owner)
{
	// 魔法陣
	m_load[TextureType::Circle].m_tex   = std::make_shared<KdTexture>();                    // テクスチャ生成
	m_load[TextureType::Circle].m_tex->Load("Asset/Textures/Loading/magic circle red.png"); // テクスチャ読み込み
	m_load[TextureType::Circle].m_rect  = { 0,0,long(993),long(993) };                      // 切り取り範囲
	m_load[TextureType::Circle].m_size  = 0.1f;                                             // 大きさ
	m_load[TextureType::Circle].m_pos   = { 640.0f - 60.0f,-360.0f + 60.0f };               // 座標
	m_load[TextureType::Circle].m_alpha = 1.0f;                                             // アルファ値
	m_load[TextureType::Circle].m_color = { 1,1,1,m_load[0].m_alpha };                      // 色
	m_load[TextureType::Circle].m_pivot = { 0.5f,0.5f };                                    // 中心位置

	// 文字
	m_load[TextureType::String].m_tex   = std::make_shared<KdTexture>();                                                                 // テクスチャ生成
	m_load[TextureType::String].m_tex->Load("Asset/Textures/Loading/NowLoading.png");													 // テクスチャ読み込み
	m_load[TextureType::String].m_rect  = { 0,0,long(578),long(75) };																	 // 切り取り範囲
	m_load[TextureType::String].m_size  = 1.0f;																							 // 大きさ
	m_load[TextureType::String].m_pos   = { m_load[0].m_pos.x - ((m_load[0].m_rect.width * m_load[0].m_size) / 2.0f),-360.0f + 10.0f };  // 座標
	m_load[TextureType::String].m_alpha = 1.0f;																							 // アルファ値
	m_load[TextureType::String].m_color = { 1,1,1,m_load[1].m_alpha };																	 // 色
	m_load[TextureType::String].m_pivot = { 1.0f,0.0f };
}

void Loading::GameLoad::Update(std::shared_ptr<Loading> owner)
{
	// 魔法陣回転
	m_load[TextureType::Circle].m_angle += m_CircleChangeAngle;
	// 360度を越えたら0度に戻す
	if (m_load[TextureType::Circle].m_angle > 360.0f)m_load[TextureType::Circle].m_angle -= 360.0f;

	// 文字アルファ値変更
	// sinカーブ用角度
	static float _angle = 0.0f;
	// 角度変更
	_angle += m_StringChangeAlpha;
	// 360度を越えたら0度に戻す
	if (_angle > 360.0f)_angle -= 360.0f;
	// アルファ値
	m_load[TextureType::String].m_alpha = sin(DirectX::XMConvertToRadians(_angle)) + m_StringCorrectAlpha;
	// 色
	m_load[TextureType::String].m_color = { 1,1,1,m_load[TextureType::String].m_alpha };

	// 行列
	for (auto& load : m_load)
	{
		// 拡縮
		Math::Matrix _scale = Math::Matrix::CreateScale(load.m_size);
		// 回転
		Math::Matrix _rot = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(load.m_angle));
		// 座標
		Math::Matrix _trans = Math::Matrix::CreateTranslation(Math::Vector3{ load.m_pos.x,load.m_pos.y,0.0f });
		// 行列合成
		load.m_Mat = _scale * _rot * _trans;
	}
}

void Loading::GameLoad::DrawSprite(std::shared_ptr<Loading> owner)
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		// 黒背景
		Math::Color _color = { 0,0,0,1 };
		KdShaderManager::Instance().m_spriteShader.DrawBox(0, 0, 640, 360, &_color, true);

		// 描画
		for (auto& load : m_load)
		{
			// 行列セット
			KdShaderManager::Instance().m_spriteShader.SetMatrix(load.m_Mat);
			KdShaderManager::Instance().m_spriteShader.DrawTex(load.m_tex, 0, 0, &load.m_rect, &load.m_color, load.m_pivot);
			// 行列を元に戻す
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
		}
	}
	KdShaderManager::Instance().m_spriteShader.End();
}
//=================================================================================================