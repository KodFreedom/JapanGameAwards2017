//--------------------------------------------------------------------------------
//
//　meshCube.cpp
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
#include "mode.h"
#include "textureManager.h"
#include "gameObject3D.h"
#include "meshCube.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CMeshCube::CMeshCube() : CGameObject3D()
	, m_vSize(CKFVec3(1.0f))
	, m_cColor(CKFColor(1.0f))
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
HRESULT	CMeshCube::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vSize, const CKFColor &cColor)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_vSize = vSize;
	m_cColor = cColor;
	m_texName = CTextureManager::TEX_TEST;

	HRESULT hr = CGameObject3D::Init(6 * 4, 6 * 4 + 5 * 2, 6 * 2 + 5 * 4);
	if (FAILED(hr)) { return hr; }
	MakeVertex();
	return hr;
}

//--------------------------------------------------------------------------------
//  頂点生成
//--------------------------------------------------------------------------------
void CMeshCube::MakeVertex(void)
{
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vHalfSize = m_vSize * 0.5f;
	int nCntVtx = 0;

	//正面
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, -1.0f);
		nCntVtx++;
	}

	//上
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY,
			vHalfSize.m_fZ - (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		nCntVtx++;
	}

	//左
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ - (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(-1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//下
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			-vHalfSize.m_fY,
			-vHalfSize.m_fZ + (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//右
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ + (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//後ろ
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX - (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//仮想アドレス解放
	m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < 6 * 4 + 5 * 2; nCnt++)
	{
		if (nCnt % 6 < 4)
		{
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 6) % 4;
		}
		else
		{//縮退
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 2) + 3;
		}
	}

	m_pIdxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
CMeshCube *CMeshCube::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vSize, const CKFColor &cColor)
{
	CMeshCube *pCube = NULL;
	pCube = new CMeshCube;
	pCube->Init(vPos, vRot, vSize, cColor);
	pCube->m_pri = GOM::PRI_3D;
	pCube->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(pCube->m_pri, pCube);
	return pCube;
}