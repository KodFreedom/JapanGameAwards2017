//--------------------------------------------------------------------------------
//
//　modelManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "modelManager.h"
#include "model.h"
#include "modelX.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
CModelManager::MODEL_INFO CModelManager::m_apDemoModelInfo[DEMO_MODEL_MAX] =
{
	{ CModelManager::XFILE, "data/MODEL/cube.x" },				//cube
	{ CModelManager::XFILE, "data/MODEL/selectionBox.x" },		//selection box
	{ CModelManager::XFILE, "data/MODEL/stairs.x" },			//stairs
	{ CModelManager::XFILE, "data/MODEL/goal.x" },				//goal
};

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModelManager::CModelManager()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CModelManager::Init(void)
{
	m_vectorModel.clear();
}

//--------------------------------------------------------------------------------
//  モデルの読み込み
//--------------------------------------------------------------------------------
void CModelManager::LoadAll(const CManager::MODE &mode)
{
	switch (mode)
	{
	case CManager::MODE_DEMO:
		Load(&m_apDemoModelInfo[0], DEMO_MODEL_MAX);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  モデルの破棄
//--------------------------------------------------------------------------------
void CModelManager::UnloadAll(void)
{
	for (int nCnt = 0; nCnt < (int)m_vectorModel.size(); nCnt++)
	{
		if (m_vectorModel[nCnt] != NULL)
		{
			m_vectorModel[nCnt]->Uninit();
			delete m_vectorModel[nCnt];
			m_vectorModel[nCnt] = NULL;
		}
	}

	m_vectorModel.clear();
}

//--------------------------------------------------------------------------------
//  モデルの取得
//--------------------------------------------------------------------------------
CModel *CModelManager::GetModel(const int &nModelID)
{
	if (nModelID < 0 || nModelID >= (int)m_vectorModel.size()) { return NULL; }

	return m_vectorModel[nModelID];
}

//--------------------------------------------------------------------------------
//  モデルの読み込み
//--------------------------------------------------------------------------------
void CModelManager::Load(MODEL_INFO *pModelBegin, const int &nModelMax)
{
	for (int nCnt = 0; nCnt < nModelMax; nCnt++)
	{
		CModel *pModel = NULL;
		switch (pModelBegin->type)
		{
		case XFILE:
			pModel = CModelX::Create(pModelBegin->path);
			break;
		case XFILE_MOTION:
			break;
		default:
			break;
		}

		m_vectorModel.push_back(pModel);
		pModelBegin++;
	}
}