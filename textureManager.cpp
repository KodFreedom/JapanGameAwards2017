//--------------------------------------------------------------------------------
//
//　textureManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
LPCSTR CTextureManager::m_apDemoTexPath[DEMO_TEX_MAX] = {
	"data/TEXTURE/polygon.jpg",					//Test Texture
	"data/TEXTURE/polygon.png",					//Test Texture
	"data/TEXTURE/road_stone.jpg",				//Field Texture
};

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CTextureManager::CTextureManager()
{
	
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CTextureManager::Init(void)
{
	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  全てのテクスチャを読み込む
//--------------------------------------------------------------------------------
void CTextureManager::LoadAll(const CManager::MODE &mode)
{
	switch (mode)
	{
	case CManager::MODE_DEMO:
		Load(&m_apDemoTexPath[0], DEMO_TEX_MAX);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  全てのテクスチャを破棄する
//--------------------------------------------------------------------------------
void CTextureManager::UnloadAll(void)
{
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
//  テクスチャを取得する
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(const int &nTexID)
{
	if (nTexID < 0 || nTexID >= (int)m_vectorTexture.size()) { return NULL; }

	return m_vectorTexture[nTexID];
}

//--------------------------------------------------------------------------------
//  テクスチャを確保する
//--------------------------------------------------------------------------------
void CTextureManager::Load(LPCSTR *pPathBegin, const int &nTexMax)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	char aStr[128];
	HRESULT hr;
	LPCSTR *pPath = pPathBegin;

	if (!m_vectorTexture.empty())
	{
		wsprintf(aStr, "m_listTextureが空じゃない!");
		MessageBox(NULL, aStr, "エラー", MB_OK | MB_ICONWARNING);
		return;
	}

	for (int nCnt = 0; nCnt < nTexMax; nCnt++)
	{
		if (pPath == NULL)
		{
			wsprintf(aStr, "%d番のテクスチャパスがNULL!", nCnt);
			MessageBox(NULL, aStr, "エラー", MB_OK | MB_ICONWARNING);
		}
		else
		{
			LPDIRECT3DTEXTURE9 pTex = NULL;
			hr = D3DXCreateTextureFromFile(pDevice, *pPath, &pTex);

			if (FAILED(hr))
			{
				wsprintf(aStr, "%d番のテクスチャの読み込みが失敗！！！", nCnt);
				MessageBox(NULL, aStr, "エラー", MB_OK | MB_ICONWARNING);
			}

			m_vectorTexture.push_back(pTex);
		}

		pPath++;
	}
}