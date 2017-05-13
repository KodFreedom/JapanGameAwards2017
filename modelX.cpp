//--------------------------------------------------------------------------------
//
//　modelX.cpp
//	Author : Xu Wenjie
//	Date   : 2017-01-23
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
#include "model.h"
#include "modelX.h"

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModelX::CModelX() : CModel()
	, m_pMesh(NULL)
	, m_pBufferMaterial(NULL)
	, m_dwNumMaterial(0)
{
	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
HRESULT CModelX::Init(const LPCSTR &pXFilePath)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//ハードディスクからXファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX(
		pXFilePath,				//ファイル名
		D3DXMESH_MANAGED,
		pDevice,
		NULL,					//隣接情報
		&m_pBufferMaterial,		//モデルのマテリアル情報
		NULL,
		&m_dwNumMaterial,		//モデルのマテリアル数
		&m_pMesh);				//メッシュ情報

	if (FAILED(hr))
	{
		MessageBox(NULL, "CModelX : D3DXLoadMeshFromX ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return hr;
	}

	//texture
	D3DXMATERIAL *pMat;//マテリアル情報を受け取れる用のポインタ
	
	pMat = (D3DXMATERIAL*)m_pBufferMaterial->GetBufferPointer();
	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			LPDIRECT3DTEXTURE9 pTexture = NULL;

			//ハードディスクからTextureの読み込み
			//※エラーチェック必須
			hr = D3DXCreateTextureFromFile(pDevice,
				pMat[nCnt].pTextureFilename,
				&pTexture);

			if (FAILED(hr))
			{
				char aStr[128];
				wsprintf(aStr, "CModelX %d番のテクスチャ ERROR!", nCnt);
				MessageBox(NULL, aStr, "エラー", MB_OK | MB_ICONWARNING);
			}

			m_vectorTexture.push_back(pTexture);
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModelX::Uninit(void)
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (m_pBufferMaterial != NULL)
	{
		m_pBufferMaterial->Release();
		m_pBufferMaterial = NULL;
	}

	for (int nCnt = 0; nCnt < (int)m_vectorTexture.size(); nCnt++)
	{
		if (m_vectorTexture[nCnt] != NULL)
		{
			m_vectorTexture[nCnt]->Release();
			m_vectorTexture[nCnt] = NULL;
		}
	}

	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CModelX::Draw(LPDIRECT3DDEVICE9 pDevice)
{
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);//現在デバイスに設定されてるマテリアル情報を取得
	D3DXMATERIAL *pMat;//マテリアル情報を受け取れる用のポインタ
	pMat = (D3DXMATERIAL*)m_pBufferMaterial->GetBufferPointer();

	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCnt].MatD3D);
		
		//Texture
		pDevice->SetTexture(0, m_vectorTexture[nCnt]);

		//メッシュの描画
		m_pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  描画処理(マテリアル指定)
//--------------------------------------------------------------------------------
void CModelX::Draw(LPDIRECT3DDEVICE9 pDevice, const CMM::MATERIAL &matType)
{
	//現在デバイスに設定されてるマテリアル情報を取得
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// マテリアルの設定
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		//Texture
		pDevice->SetTexture(0, m_vectorTexture[nCnt]);

		//メッシュの描画
		m_pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  描画処理(マテリアル指定, テクスチャ指定)
//--------------------------------------------------------------------------------
void CModelX::Draw(LPDIRECT3DDEVICE9 pDevice, const CMM::MATERIAL &matType, const int &nTexID)
{
	//現在デバイスに設定されてるマテリアル情報を取得
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// マテリアルの設定
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(nTexID);
	pDevice->SetTexture(0, pTexture);

	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		//メッシュの描画
		m_pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  作成
//--------------------------------------------------------------------------------
CModelX *CModelX::Create(const LPCSTR pXFilePath)
{
	CModelX *pModelX = NULL;
	pModelX = new CModelX;
	pModelX->Init(pXFilePath);
	return pModelX;
}
