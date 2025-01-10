#pragma once

// UI基底
#include"../../UIBase.h"

// パーティクル
class Particle;

class Cursor :public UIBase
{
public:
	// パーティクル構造体
	struct ParticleParam
	{
		// リスト
		std::vector<std::shared_ptr<Particle>> ParticleList;

		// 最大数
		int ParticleNum = 10;

		// 移動量
		int MoveXNum = 0; // X軸のランダムの数
		int MoveYNum = 0; // Y軸のランダムの数
		Math::Vector2 MinMove = { 0.0f,0.0f }; // 移動量の最低値
		Math::Vector2 MoveCorrection = { 0.0f,0.0f }; // 移動量の補正値

		// サイズ
		int SizeNum = 0;             // ランダムの数
		float MinSize = 0.0f;        // 最低値
		float SizeCorrection = 0.0f; // 補正値

		// サイズ変化量
		float SizeChange = 0.0f;
	};

	Cursor() {};
	~Cursor()override {};

	void Update()    override;
	void PostUpdate()override;
	void DrawSprite()override;
	void Init()      override;

	void ParticleDelete(); // パーティクル削除

	// セッター======================================================================================================================================
	// パーティクル
	void SetParticleParam(int pNum, int pMoveXNum, int pMoveYNum, Math::Vector2 pMinMove, Math::Vector2 pMoveCorrection, int pSizeNum, float pMinSize, float pSizeCorrection, float pSizeChange)
	{
		// 最大数
		m_particle.ParticleNum    = pNum;
		// X軸のランダムの数
		m_particle.MoveXNum       = pMoveXNum;
		// Y軸のランダムの数
		m_particle.MoveYNum       = pMoveYNum;
		// 移動量の最低値
		m_particle.MinMove        = pMinMove;
		// 移動量の補正値
		m_particle.MoveCorrection = pMoveCorrection;
		// サイズのランダムの数
		m_particle.SizeNum        = pSizeNum;
		// サイズ最低値
		m_particle.MinSize        = pMinSize;
		// サイズ補正値
		m_particle.SizeCorrection = pSizeCorrection;
		// サイズ変化量
		m_particle.SizeChange     = pSizeChange;
	}
	//==================================================================================================================================================

	// ゲッター=========================================================================================================================================
	const ParticleParam& GetParticleParam()const { return m_particle; } // パーティクル
	//==================================================================================================================================================

private:
	bool          m_ClickFlg = false; // クリック確認フラグ　※長押し防止

	ParticleParam m_particle;         // パーティクル
};