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
#include "stageEditor.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define STAGE_BIN_FILE_NAME "stage.bin"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CStageEditor::CStageEditor() : CStage()
, m_nActLayer(0)
, m_bViewMode(false)
{
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CStageEditor::~CStageEditor()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
HRESULT CStageEditor::Init(const CKFVec3 &vPos, const CKFVec3 &vRot)
{
	m_vPos = vPos;
	m_vRot = vRot;

	//操作できるレイヤー初期化
	m_nActLayer = (m_nNumBlockPerAxis - 1) / 2;

	int nHalf = m_nNumBlockPerAxis / 2;
	CKFVec3 vStart = CKFVec3(nHalf * -1.0f, nHalf * 1.0f, nHalf * -1.0f);
	for (int nCntZ = 0; nCntZ < m_nNumBlockPerAxis; nCntZ++)
	{
		for (int nCntY = 0; nCntY < m_nNumBlockPerAxis; nCntY++)
		{
			for (int nCntX = 0; nCntX < m_nNumBlockPerAxis; nCntX++)
			{
				BLOCK block;

				int nNo = nCntZ * m_nNumBlockPerAxis * m_nNumBlockPerAxis + nCntY * m_nNumBlockPerAxis + nCntX;
				block.type = (nNo % 2) == 0 ? BT_WHITE : BT_BLACK;
				block.status = BS_NONE;
				block.vPos = vStart + CKFVec3(nCntX * 1.0f, nCntY * -1.0f, nCntZ * 1.0f);
				block.nLayer = max(abs(nCntZ - nHalf), max(abs(nCntY - nHalf), abs(nCntX - nHalf)));

				//リストに保存
				m_listBlock.push_back(block);
			}
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CStageEditor::Uninit(void)
{
	m_listBlock.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CStageEditor::Update(void)
{
	CKeyboardDX *pKeyboard = GetManager()->GetKeyboard();

	//Save
	if (pKeyboard->GetKeyTrigger(DIK_F9))
	{
		SaveStage();
	}

	//Load
	if (pKeyboard->GetKeyTrigger(DIK_F11))
	{
		LoadStage();
	}

	//モードの切り替え
	if (pKeyboard->GetKeyTrigger(DIK_V))
	{
		m_bViewMode ^= 1;
	}

	if (m_bViewMode)
	{
		m_selectionBox.bSelected = false;
		m_selectionBox.pChoosenBlock = NULL;

		//ステータス更新
		std::list<BLOCK>::iterator itr;
		for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
		{
			itr->status = BS_NONE;
		}
	}
	else
	{
		if (m_selectionBox.bSelected)
		{
			//Selection Boxの更新
			UpdateSelectionBox();
		}
		else
		{
			//操作できるレイヤーの変更
			if (pKeyboard->GetKeyTrigger(DIK_UP))
			{
				m_nActLayer = (m_nActLayer + 1) % ((m_nNumBlockPerAxis - 1) / 2 + 1);
			}

			if (pKeyboard->GetKeyTrigger(DIK_DOWN))
			{
				m_nActLayer--;
				m_nActLayer = m_nActLayer < 0 ? (m_nNumBlockPerAxis - 1) / 2 : m_nActLayer;
			}

			//マウスレイとブロックの当たり判定更新
			UpdateChooseBlock();
		}

		//ステータス更新
		std::list<BLOCK>::iterator itr;
		for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
		{
			//レイトの当たり判定(操作できるレイヤーのみ)
			if (itr->nLayer == m_nActLayer)
			{
				itr->status = BS_NONE;
			}
			else
			{
				itr->status = BS_DIS_CHOOSE;
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CStageEditor::LateUpdate(void)
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
void CStageEditor::Draw(void)
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
void CStageEditor::UpdateSelectionBox(void)
{
	CMouseDX *pMouse = GetManager()->GetMouse();
	CKeyboardDX *pKeyboard = GetManager()->GetKeyboard();

	//ブロックのタイプ変更
	if (pKeyboard->GetKeyTrigger(DIK_UP))
	{
		m_selectionBox.pChoosenBlock->type = (BLOCK_TYPE)(((int)m_selectionBox.pChoosenBlock->type + 1) % (int)BT_MAX);
	}
	else if (pKeyboard->GetKeyTrigger(DIK_DOWN))
	{
		m_selectionBox.pChoosenBlock->type = (BLOCK_TYPE)(m_selectionBox.pChoosenBlock->type == BT_NONE ? BT_MAX - 1 : m_selectionBox.pChoosenBlock->type - 1);
	}

	//ブロックの回転の変更
	//X軸
	if (pKeyboard->GetKeyTrigger(DIK_W))
	{
		m_selectionBox.pChoosenBlock->vRot.m_fX += KF_PI * 0.5f;
		m_selectionBox.pChoosenBlock->vRot.m_fX = m_selectionBox.pChoosenBlock->vRot.m_fX >= KF_PI * 2.0f ? 0.0f : m_selectionBox.pChoosenBlock->vRot.m_fX;
	}
	else if (pKeyboard->GetKeyTrigger(DIK_S))
	{
		m_selectionBox.pChoosenBlock->vRot.m_fX -= KF_PI * 0.5f;
		m_selectionBox.pChoosenBlock->vRot.m_fX = m_selectionBox.pChoosenBlock->vRot.m_fX <= -KF_PI * 2.0f ? 0.0f : m_selectionBox.pChoosenBlock->vRot.m_fX;
	}

	//Y軸
	if (pKeyboard->GetKeyTrigger(DIK_A))
	{
		m_selectionBox.pChoosenBlock->vRot.m_fY += KF_PI * 0.5f;
		m_selectionBox.pChoosenBlock->vRot.m_fY = m_selectionBox.pChoosenBlock->vRot.m_fY >= KF_PI * 2.0f ? 0.0f : m_selectionBox.pChoosenBlock->vRot.m_fY;
	}
	else if (pKeyboard->GetKeyTrigger(DIK_D))
	{
		m_selectionBox.pChoosenBlock->vRot.m_fY -= KF_PI * 0.5f;
		m_selectionBox.pChoosenBlock->vRot.m_fY = m_selectionBox.pChoosenBlock->vRot.m_fY <= -KF_PI * 2.0f ? 0.0f : m_selectionBox.pChoosenBlock->vRot.m_fY;
	}

	//Z軸
	if (pKeyboard->GetKeyTrigger(DIK_Q))
	{
		m_selectionBox.pChoosenBlock->vRot.m_fZ += KF_PI * 0.5f;
		m_selectionBox.pChoosenBlock->vRot.m_fZ = m_selectionBox.pChoosenBlock->vRot.m_fZ >= KF_PI * 2.0f ? 0.0f : m_selectionBox.pChoosenBlock->vRot.m_fZ;
	}
	else if (pKeyboard->GetKeyTrigger(DIK_E))
	{
		m_selectionBox.pChoosenBlock->vRot.m_fZ -= KF_PI * 0.5f;
		m_selectionBox.pChoosenBlock->vRot.m_fZ = m_selectionBox.pChoosenBlock->vRot.m_fZ <= -KF_PI * 2.0f ? 0.0f : m_selectionBox.pChoosenBlock->vRot.m_fZ;
	}

	//左クリックしたら選択したブロックを解放する
	if (pMouse->GetMouseTrigger(CMouseDX::MOUSE_LEFT))
	{
		m_selectionBox.bSelected = false;
	}
}

//--------------------------------------------------------------------------------
//  マウスレイとブロックの当たり判定更新
//--------------------------------------------------------------------------------
void CStageEditor::UpdateChooseBlock(void)
{
	//レイ算出
	CMouseDX *pMouse = GetManager()->GetMouse();
	POINT pMousePos = pMouse->GetMousePos();
	CKFRay ray = CKFUtilityDX::ChangeToPickingRay(CKFVec2((float)pMousePos.x, (float)pMousePos.y));

	//計算用変数
	CKFMath::RTS_INFO rtsMin;
	rtsMin.bIsContact = false;
	rtsMin.fTimingMin = 0.0f;
	m_selectionBox.pChoosenBlock = NULL;

	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		//レイトの当たり判定(操作できるレイヤーのみ)
		//距離が一番近いのブロックを選ぶ
		if (itr->nLayer == m_nActLayer)
		{
			CKFMath::RTS_INFO rts = CKFMath::ContactRayToSphere(ray, itr->vPos, 0.5f);
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

	//左クリックしたら選択したブロックをロックする(もう一回左クリックしないと外せない)
	if (m_selectionBox.pChoosenBlock != NULL && pMouse->GetMouseTrigger(CMouseDX::MOUSE_LEFT))
	{
		m_selectionBox.bSelected = true;
	}
}

//--------------------------------------------------------------------------------
//  ブロック描画
//--------------------------------------------------------------------------------
void CStageEditor::DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage)
{
	CModel *pModel = NULL;
	CMM::MATERIAL mat = CMM::MAT_NORMAL;
	CTM::TEX_NAME texture = CTM::TEX_MAX;

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
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_CUBE);
		mat = CMM::MAT_WHITE;
		break;
	case BT_BLACK:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_CUBE);
		mat = CMM::MAT_BLACK;
		break;
	case BT_STAIRS:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_STAIRS);
		mat = CMM::MAT_GRAY;
		break;
	case BT_GOAL:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_GOAL);
		mat = CMM::MAT_GOLDEN;
		break;
	default:
		return;
	}

	switch (block.status)
	{
	case BS_NONE:
		pModel->Draw(mat);
		break;
	case BS_DIS_CHOOSE:
		texture = CTM::TEX_POLYGON_ALPHA;
		pModel->Draw(mat, texture);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  Selection Box描画
//--------------------------------------------------------------------------------
void CStageEditor::DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage)
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

	CModel *pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_SELECTION_BOX);
	CMM::MATERIAL mat = m_selectionBox.bSelected ? CMM::MAT_RED_CHOOSEN : CMM::MAT_RED;
	pModel->Draw(mat);
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
CStageEditor *CStageEditor::Create(const CKFVec3 &vPos, const CKFVec3 &vRot)
{
	CStageEditor *pStage = NULL;
	pStage = new CStageEditor;
	pStage->Init(vPos, vRot);
	pStage->m_pri = GOM::PRI_3D;
	pStage->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(pStage->m_pri, pStage);
	return pStage;
}

//--------------------------------------------------------------------------------
//  ステージの書き出し
//--------------------------------------------------------------------------------
void CStageEditor::SaveStage(void)
{
	FILE *pFile;

	//bin file出力
	fopen_s(&pFile, STAGE_BIN_FILE_NAME, "wb");

	if (pFile != NULL)
	{
		std::list<BLOCK>::iterator itr;
		for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
		{
			fwrite(&(*itr), sizeof(BLOCK), 1, pFile);
		}

		fclose(pFile);
	}
}

//--------------------------------------------------------------------------------
//  ステージの読み込み
//--------------------------------------------------------------------------------
void CStageEditor::LoadStage(void)
{
	FILE *pFile;

	//bin file
	fopen_s(&pFile, STAGE_BIN_FILE_NAME, "rb");

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