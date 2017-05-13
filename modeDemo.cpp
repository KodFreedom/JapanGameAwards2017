//--------------------------------------------------------------------------------
//
//　modeDemo.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "mode.h"
#include "modeDemo.h"
#include "gameObject.h"
#include "gameObject3D.h"
#include "gameObjectModel.h"
#include "meshField.h"
#include "meshCube.h"
#include "stage.h"
#include "stageEditor.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModeDemo::CModeDemo() : CMode()
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CModeDemo::~CModeDemo()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
HRESULT CModeDemo::Init(void)
{
	//テクスチャの読み込み
	GetManager()->GetTextureManager()->LoadAll(CManager::MODE_DEMO);
	
	//モデルの読み込み
	GetManager()->GetModelManager()->LoadAll(CManager::MODE_DEMO);

	//ゲームオブジェクトの初期化
	//CMeshField::Create(1, 1, CKFVec2(1.0f, 1.0f), CKFVec3(0.0f), CKFVec3(0.0f));
	//CMeshCube::Create(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f), CKFColor(1.0f));
	//CGameObjectModel::Create(CKFVec3(0.0f), CKFVec3(0.0f), CModelManager::DEMO_MODEL_CUBE);
	CStageEditor::Create(CKFVec3(0.0f), CKFVec3(0.0f));

	//ライトの初期化
	GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CCamera;
	m_pCamera->Init();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModeDemo::Uninit(void)
{
	//ゲームオブジェクトの破棄
	CMode::Uninit();

	//ライトの破棄
	GetManager()->GetLightManager()->ReleaseAll();

	//カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//モデルの破棄
	GetManager()->GetModelManager()->UnloadAll();

	//テクスチャの破棄
	GetManager()->GetTextureManager()->UnloadAll();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();

	m_pCamera->Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();

	m_pCamera->LateUpdate();
}