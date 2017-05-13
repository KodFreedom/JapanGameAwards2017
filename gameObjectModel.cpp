//--------------------------------------------------------------------------------
//
//　gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "mode.h"
#include "modelManager.h"
#include "model.h"
#include "gameObject.h"
#include "gameObjectModel.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectModel::CGameObjectModel() : CGameObject()
	, m_vRot(CKFVec3(0.0f))
	, m_nModelID(-1)
	, m_matType(CMM::MAT_WHITE)
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CGameObjectModel::~CGameObjectModel()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
HRESULT CGameObjectModel::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nModelID)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_nModelID = nModelID;

	return S_OK;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CGameObjectModel::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CGameObjectModel::Update(void)
{

}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CGameObjectModel::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CGameObjectModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	CModel *pModel = GetManager()->GetModelManager()->GetModel(m_nModelID);

	if (pModel != NULL)
	{
		//RenderState設定
		SetRenderState(pDevice);

		//マトリックス設定
		SetMatrix(pDevice);

		//描画
		if (m_matType != CMM::MAT_MAX)
		{
			pModel->Draw(pDevice, m_matType);
		}
		else
		{
			pModel->Draw(pDevice);
		}

		//RenderState戻す
		ResetRenderState(pDevice);
	}
}

//--------------------------------------------------------------------------------
// SetWorldMatrix
//--------------------------------------------------------------------------------
void CGameObjectModel::SetMatrix(LPDIRECT3DDEVICE9 pDevice)
{
	//ワールド相対座標
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//単位行列に初期化
	D3DXMatrixIdentity(&mtxWorld);

	//回転(Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);
	mtxWorld *= mtxRot;

	//平行移動
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);
	mtxWorld *= mtxPos;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//--------------------------------------------------------------------------------
// SetRenderState
//--------------------------------------------------------------------------------
void CGameObjectModel::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{

}

//--------------------------------------------------------------------------------
// ResetRenderState
//--------------------------------------------------------------------------------
void CGameObjectModel::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{

}

//--------------------------------------------------------------------------------
// 生成
//--------------------------------------------------------------------------------
CGameObjectModel *CGameObjectModel::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nModelID)
{
	CGameObjectModel *pObjModel = NULL;
	pObjModel = new CGameObjectModel;
	pObjModel->Init(vPos, vRot, nModelID);
	pObjModel->m_pri = CMode::PRI_3D;
	pObjModel->m_nID = GetManager()->GetModeNow()->SaveGameObj(CMode::PRI_3D, pObjModel);
	return pObjModel;
}