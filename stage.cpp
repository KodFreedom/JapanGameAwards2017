//--------------------------------------------------------------------------------
//	ステージ
//　stage.cpp
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
#include "manager.h"
#include "materialManager.h"
#include "textureManager.h"
#include "rendererDX.h"
#include "inputDX.h"
#include "camera.h"
#include "mode.h"
#include "modelManager.h"
#include "model.h"
#include "gameObject.h"
#include "stage.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CStage::CStage() : CGameObject()
	, m_vRot(0.0f)
{
	m_selectionBox.bSelected = false;
	m_selectionBox.pChoosenBlock = NULL;
	m_listBlock.clear();
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CStage::~CStage()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
HRESULT CStage::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const LPSTR &stagePath)
{
	m_vPos = vPos;
	m_vRot = vRot;

	LoadStage(stagePath);

	return S_OK;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CStage::Uninit(void)
{
	m_listBlock.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CStage::Update(void)
{
	CKeyboardDX *pKeyboard = GetManager()->GetKeyboard();

	//マウスレイとブロックの当たり判定更新
	UpdateChooseBlock();

	//ステータス更新
	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		itr->status = BS_NONE;
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CStage::LateUpdate(void)
{
	CCamera *pCamera = GetManager()->GetModeNow()->GetCamera();
	CKFVec3 vCameraEye = pCamera->GetPosEye();
	CKFVec3 vCameraAt = pCamera->GetPosAt();

	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		//Z深度更新
		itr->fZDepth = CKFMath::CalculateZDepth(itr->vPos, vCameraEye, vCameraAt);
	}

	//Zソート
	m_listBlock.sort();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CStage::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//ステージの行列算出
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//回転(Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);

	//平行移動
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);

	//Selection Boxの描画
	DrawSelectionBox(pDevice, mtxRot, mtxPos);

	//ブロックの描画
	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		DrawBlock(*itr, pDevice, mtxRot, mtxPos);
	}
}

//--------------------------------------------------------------------------------
//  マウスレイとブロックの当たり判定更新
//--------------------------------------------------------------------------------
void CStage::UpdateSelectionBox(void)
{

}

//--------------------------------------------------------------------------------
//  マウスレイとブロックの当たり判定更新
//--------------------------------------------------------------------------------
void CStage::UpdateChooseBlock(void)
{
	//レイ算出
	CMouseDX *pMouse = GetManager()->GetMouse();
	POINT pMousePos = pMouse->GetMousePos();
	CCamera *pCamera = GetManager()->GetModeNow()->GetCamera();
	D3DXMATRIX mtx = pCamera->GetMtxViewInverse();
	CKFRay ray = CKFMath::CalculatePickingRay(CKFVec2((float)pMousePos.x, (float)pMousePos.y), mtx);

	//計算用変数
	CKFMath::RTS_INFO rtsMin;
	rtsMin.bIsContact = false;
	rtsMin.fTimingMin = 0.0f;
	m_selectionBox.pChoosenBlock = NULL;

	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		//レイトの当たり判定
		//距離が一番近いのブロックを選ぶ
		CKFMath::RTS_INFO rts = CKFMath::ContactRaytoSphere(ray, itr->vPos, 0.5f);
		if (rts.bIsContact)
		{
			if (m_selectionBox.pChoosenBlock == NULL)
			{
				m_selectionBox.pChoosenBlock = &(*itr);
				rtsMin = rts;
			}
			else
			{
				if (rts.fTimingMin < rtsMin.fTimingMin)
				{
					m_selectionBox.pChoosenBlock = &(*itr);
					rtsMin = rts;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  ブロック描画
//--------------------------------------------------------------------------------
void CStage::DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage)
{
	CModel *pModel = NULL;
	CMM::MATERIAL mat = CMM::MAT_NORMAL;
	CTM::DEMO_TEX_NAME texture = CTM::DEMO_TEX_MAX;

	//ワールド相対座標
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//単位行列に初期化
	D3DXMatrixIdentity(&mtxWorld);

	//回転(Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, block.vRot.m_fY, block.vRot.m_fX, block.vRot.m_fZ);
	mtxWorld *= mtxRot;

	//平行移動
	D3DXMatrixTranslation(&mtxPos, block.vPos.m_fX, block.vPos.m_fY, block.vPos.m_fZ);
	mtxWorld *= mtxPos;

	//ステージ行列にかける
	mtxWorld *= mtxRotStage;
	mtxWorld *= mtxPosStage;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//描画
	switch (block.type)
	{
	case BT_NONE:
		return;
	case BT_WHITE:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_CUBE);
		mat = CMM::MAT_WHITE;
		break;
	case BT_BLACK:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_CUBE);
		mat = CMM::MAT_BLACK;
		break;
	case BT_STAIRS:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_STAIRS);
		mat = CMM::MAT_GRAY;
		break;
	case BT_GOAL:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_GOAL);
		mat = CMM::MAT_GOLDEN;
		break;
	default:
		return;
	}

	switch (block.status)
	{
	case BS_NONE:
		pModel->Draw(pDevice, mat);
		break;
	case BS_DIS_CHOOSE:
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  Selection Box描画
//--------------------------------------------------------------------------------
void CStage::DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage)
{
	if (m_selectionBox.pChoosenBlock == NULL) { return; }

	//ワールド相対座標
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxPos;

	//単位行列に初期化
	D3DXMatrixIdentity(&mtxWorld);

	//平行移動
	D3DXMatrixTranslation(&mtxPos, 
		m_selectionBox.pChoosenBlock->vPos.m_fX, 
		m_selectionBox.pChoosenBlock->vPos.m_fY, 
		m_selectionBox.pChoosenBlock->vPos.m_fZ);
	mtxWorld *= mtxPos;

	//ステージ行列にかける
	mtxWorld *= mtxRotStage;
	mtxWorld *= mtxPosStage;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	CModel *pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_SELECTION_BOX);
	CMM::MATERIAL mat = m_selectionBox.bSelected ? CMM::MAT_RED_CHOOSEN : CMM::MAT_RED;
	pModel->Draw(pDevice, mat);
}

//--------------------------------------------------------------------------------
//  ステージの読み込み
//--------------------------------------------------------------------------------
void CStage::LoadStage(const LPSTR &stagePath)
{
	FILE *pFile;

	//bin file
	fopen_s(&pFile, stagePath, "rb");

	if (pFile != NULL)
	{
		int nNumBlock = m_listBlock.size();
		m_listBlock.clear();

		BLOCK block;
		for (int nCnt = 0; nCnt < nNumBlock; nCnt++)
		{
			fread(&block, sizeof(BLOCK), 1, pFile);
			m_listBlock.push_back(block);
		}

		fclose(pFile);
	}
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
CStage *CStage::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const LPSTR &stagePath)
{
	CStage *pStage = NULL;
	pStage = new CStage;
	pStage->Init(vPos, vRot, stagePath);
	pStage->m_pri = CMode::PRI_3D;
	pStage->m_nID = GetManager()->GetModeNow()->SaveGameObj(CMode::PRI_3D, pStage);
	return pStage;
}