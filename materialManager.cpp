//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//　materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CMaterialManager::Init(void)
{
	m_aMaterial[MAT_NORMAL] = InitMaterial(CKFColor(1.0f)
		, CKFColor(1.0f)
		, CKFColor(0.0f, 0.0f, 0.0f, 1.0f)
		, CKFColor(0.0f, 0.0f, 0.0f, 1.0f)
		, 0.8f);

	m_aMaterial[MAT_WHITE] = InitMaterial(CKFColor(0.8f, 0.8f, 0.8f, 1.0f)
		, CKFColor(0.8f, 0.8f, 0.8f, 1.0f)
		, CKFColor(0.8f, 0.8f, 0.8f, 1.0f)
		, CKFColor(0.8f, 0.8f, 0.8f, 1.0f)
		, 0.8f);

	m_aMaterial[MAT_BLACK] = InitMaterial(CKFColor(0.1f, 0.1f, 0.1f, 1.0f)
		, CKFColor(0.1f, 0.1f, 0.1f, 1.0f)
		, CKFColor(0.1f, 0.1f, 0.1f, 1.0f)
		, CKFColor(0.1f, 0.1f, 0.1f, 1.0f)
		, 0.8f);

	m_aMaterial[MAT_GRAY] = InitMaterial(CKFColor(0.35f, 0.35f, 0.35f, 1.0f)
		, CKFColor(0.35f, 0.35f, 0.35f, 1.0f)
		, CKFColor(0.35f, 0.35f, 0.35f, 1.0f)
		, CKFColor(0.35f, 0.35f, 0.35f, 1.0f)
		, 0.8f);

	m_aMaterial[MAT_RED] = InitMaterial(CKFColor(1.0f, 0.0f, 0.0f, 1.0f)
		, CKFColor(1.0f, 0.0f, 0.0f, 1.0f)
		, CKFColor(1.0f, 0.0f, 0.0f, 1.0f)
		, CKFColor(1.0f, 0.0f, 0.0f, 1.0f)
		, 0.8f);

	m_aMaterial[MAT_RED_CHOOSEN] = InitMaterial(CKFColor(0.35f, 0.0f, 0.0f, 1.0f)
		, CKFColor(0.35f, 0.0f, 0.0f, 1.0f)
		, CKFColor(0.35f, 0.0f, 0.0f, 1.0f)
		, CKFColor(0.35f, 0.0f, 0.0f, 1.0f)
		, 0.8f);

	m_aMaterial[MAT_GOLDEN] = InitMaterial(CKFColor(0.5f, 0.5f, 0.0f, 1.0f)
		, CKFColor(0.5f, 0.5f, 0.0f, 1.0f)
		, CKFColor(0.5f, 0.5f, 0.0f, 1.0f)
		, CKFColor(0.5f, 0.5f, 0.0f, 1.0f)
		, 0.8f);
}

//--------------------------------------------------------------------------------
//  マテリアル取得
//--------------------------------------------------------------------------------
D3DMATERIAL9 CMaterialManager::GetMaterial(const MATERIAL &mat)
{
	return m_aMaterial[mat];
}

//--------------------------------------------------------------------------------
//  マテリアル設定
//--------------------------------------------------------------------------------
D3DMATERIAL9 CMaterialManager::InitMaterial(const CKFColor &cAmbient, const CKFColor &cDiffuse, const CKFColor &cSpecular, const CKFColor &cEmissive, const float &fPower)
{
	D3DMATERIAL9 mat;
	mat.Ambient = cAmbient;		// 環境光の反射率
	mat.Diffuse = cDiffuse;		// 漫射光の反射率
	mat.Specular = cSpecular;	// 鏡面光の反射率
	mat.Emissive = cEmissive;	// 自発光
	mat.Power = fPower;			// ハイライトのシャープネス
	return mat;
}