//--------------------------------------------------------------------------------
//
//　gameObject2D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "rendererDX.h"
#include "gameObject.h"
#include "gameObject2D.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObject2D::CGameObject2D()
	: m_fRot(0.0f)
	, m_vSize(CKFVec2(0.0f, 0.0f))
	, m_cColor(CKFColor(1.0f))
	, m_nTexID(-1)
{
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CGameObject2D::~CGameObject2D()
{
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
HRESULT CGameObject2D::Init(const CKFVec3 &vPos, const float &fRot, const CKFVec2 &vSize, const int &nTexID)
{
	m_vPos = vPos;
	m_fRot = fRot;
	m_vSize = vSize;
	m_nTexID = nTexID;

	HRESULT hr = MakeVertex();
	return hr;
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CGameObject2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void CGameObject2D::Update(void)
{

}

//--------------------------------------------------------------------------------
// 更新処理(描画直前)
//--------------------------------------------------------------------------------
void CGameObject2D::LateUpdate(void)
{
	UpdateVertex();
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void CGameObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	// レンダーステート設定
	SetRenderState(pDevice);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,						//ストリーム番号
		m_pVtxBuffer,			//頂点バッファ
		0,						//オフセット（開始位置）
		sizeof(VERTEX_2D));		//ストライド量

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(m_nTexID);
	pDevice->SetTexture(0, pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,						//オフセット（頂点数）
		m_nNumPolygon2D);		//ポリゴン数

	// レンダーステート戻す
	ResetRenderState(pDevice);
}

//--------------------------------------------------------------------------------
// レンダーステート設定
//--------------------------------------------------------------------------------
void CGameObject2D::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{

}

//--------------------------------------------------------------------------------
// レンダーステート戻す
//--------------------------------------------------------------------------------
void CGameObject2D::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{

}

//--------------------------------------------------------------------------------
//  頂点更新
//--------------------------------------------------------------------------------
void CGameObject2D::UpdateVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点位置更新
	SetVtxPos(pVtx);

	//頂点カラー更新
	SetVtxColor(pVtx);

	//仮想アドレス解放
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  頂点生成処理
//--------------------------------------------------------------------------------
HRESULT CGameObject2D::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * m_nNumVtx2D,	//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法
		FVF_VERTEX_2D,						//書かなくても大丈夫
		D3DPOOL_MANAGED,					//メモリ管理方法(managed：デバイスにお任せ)
		&m_pVtxBuffer,						//頂点バッファのアドレス
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CGameObject2D : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return hr;
	}

	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点位置設定
	SetVtxPos(pVtx);

	//頂点カラー設定
	SetVtxColor(pVtx);

	//頂点UV設定
	SetVtxUV(pVtx);

	//rhwの設定(必ず1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//仮想アドレス解放
	m_pVtxBuffer->Unlock();

	return hr;
}

//--------------------------------------------------------------------------------
//  頂点座標設定
//--------------------------------------------------------------------------------
void CGameObject2D::SetVtxPos(VERTEX_2D *pVtx)
{
	float fLength = CKFMath::VecMagnitude(m_vSize) * 0.5f;
	float fAngle = atan2f(m_vSize.m_fY, m_vSize.m_fX);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot + KF_PI + fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot - fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot - KF_PI - fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot + fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot + fAngle) * fLength, 0.0f);
}

//--------------------------------------------------------------------------------
//  頂点カラー設定
//--------------------------------------------------------------------------------
void CGameObject2D::SetVtxColor(VERTEX_2D *pVtx)
{
	//頂点カラーの設定(0～255の整数値)
	pVtx[0].ulColor = m_cColor;
	pVtx[1].ulColor = m_cColor;
	pVtx[2].ulColor = m_cColor;
	pVtx[3].ulColor = m_cColor;
}

//--------------------------------------------------------------------------------
//  頂点UV設定
//--------------------------------------------------------------------------------
void CGameObject2D::SetVtxUV(VERTEX_2D *pVtx)
{
	//texture頂点
	pVtx[0].vUV = CKFVec2(0.0f, 0.0f);
	pVtx[1].vUV = CKFVec2(1.0f, 0.0f);
	pVtx[2].vUV = CKFVec2(0.0f, 1.0f);
	pVtx[3].vUV = CKFVec2(1.0f, 1.0f);
}