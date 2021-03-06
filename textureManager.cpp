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
LPCSTR CTextureManager::m_apTexPath[TEX_MAX] = {
	NULL,
	"data/TEXTURE/polygon.jpg",					//Test Texture
	"data/TEXTURE/polygon.png",					//Test Texture
	"data/TEXTURE/road_stone.jpg",				//Field Texture
	"data/TEXTURE/Back.png",				//Field Texture
	"data/TEXTURE/Quick.png",				//Field Texture
	"data/TEXTURE/fade.jpg",				//Field Texture
	"data/TEXTURE/title.jpg",				//Field Texture
	"data/TEXTURE/stageclear.png",				//Field Texture
	"data/TEXTURE/white.png",				//Field Texture
	"data/TEXTURE/black.png",				//Field Texture
	"data/TEXTURE/alpha.png",				//Field Texture
	"data/TEXTURE/stage1.png",
	"data/TEXTURE/stage2.png",
	"data/TEXTURE/stage3.png",
	"data/TEXTURE/stage4.png",
	"data/TEXTURE/stage5.png",
	"data/TEXTURE/stage6.png",
	"data/TEXTURE/stage7.png",
	"data/TEXTURE/stage8.png",
	"data/TEXTURE/stage9.png",
	"data/TEXTURE/stage10.png",
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
void CTextureManager::LoadAll(void)
{
	Load(TEX_NONE, TEX_MAX);
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
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(const TEX_NAME &texName)
{
	if (texName <= TEX_NONE || texName >= TEX_MAX) { return NULL; }

	return m_vectorTexture[texName];
}

//--------------------------------------------------------------------------------
//  テクスチャを確保する
//--------------------------------------------------------------------------------
void CTextureManager::Load(const TEX_NAME &texBegin, const TEX_NAME &texEnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	LPSTR str = NULL;
	HRESULT hr;

	if (!m_vectorTexture.empty())
	{
		wsprintf(str, "m_vectorTextureが空じゃない!");
		MessageBox(NULL, str, "エラー", MB_OK | MB_ICONWARNING);
		return;
	}

	for (int nCnt = texBegin; nCnt < texEnd; nCnt++)
	{
		LPDIRECT3DTEXTURE9 pTex = NULL;

		if (m_apTexPath[nCnt] != NULL)
		{
			hr = D3DXCreateTextureFromFile(pDevice, m_apTexPath[nCnt], &pTex);

			if (FAILED(hr))
			{
				wsprintf(str, "%d番のテクスチャの読み込みが失敗！！！", nCnt);
				MessageBox(NULL, str, "エラー", MB_OK | MB_ICONWARNING);
			}
		}

		m_vectorTexture.push_back(pTex);
	}
}