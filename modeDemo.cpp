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
#include "fade.h"
#include "inputDX.h"
#include "soundManager.h"

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
void CModeDemo::Init(void)
{
	//ゲームオブジェクトの初期化
	m_pStage = CStage::Create(CKFVec3(0.0f), CKFVec3(0.0f), 0);
	//CStageEditor::Create(CKFVec3(0.0f), CKFVec3(0.0f));

	//ライトの初期化
	GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CCamera;
	m_pCamera->Init();

	GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
	m_bStartLoop = false;
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

	GetManager()->GetSoundManager()->Stop(CSM::BGM_GAME);
	GetManager()->GetSoundManager()->Stop(CSM::BGM_GAME_LOOP);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();

	m_pCamera->Update();

	CSoundManager *pSM = GetManager()->GetSoundManager();
	if (pSM->IsOver(CSM::BGM_GAME) && !m_bStartLoop)
	{
		pSM->Play(CSM::BGM_GAME_LOOP);
		m_bStartLoop = true;
	}

	CKeyboardDX *pKeyboard = GetManager()->GetKeyboard();
	if (pKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		GetManager()->GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();

	m_pCamera->LateUpdate();
}