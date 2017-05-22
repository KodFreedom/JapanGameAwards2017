//--------------------------------------------------------------------------------
//
//　mode.cpp
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
#include "mode.h"
#include "camera.h"
#include "manager.h"
#include "gameObjectManager.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CMode::CMode()
	: m_pCamera(NULL)
{
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CMode::Uninit(void)
{
	//カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//オブジェクトの破棄
	GetManager()->GetGameObjectManager()->ReleaseAll();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CMode::Update(void)
{
	//カメラの更新
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	//オブジェクトの更新
	GetManager()->GetGameObjectManager()->UpdateAll();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CMode::LateUpdate(void)
{
	//カメラの更新
	if (m_pCamera != NULL)
	{
		m_pCamera->LateUpdate();
	}

	//オブジェクトの更新
	GetManager()->GetGameObjectManager()->LateUpdateAll();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CMode::Draw(void)
{
	//カメラのセット
	if (m_pCamera != NULL)
	{
		m_pCamera->Set();
	}

	//オブジェクトの描画
	GetManager()->GetGameObjectManager()->DrawAll();
}