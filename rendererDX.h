//--------------------------------------------------------------------------------
//
//　rendererDX.h
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _RENDERER_DX_H_
#define _RENDERER_DX_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CRendererDX
{
public:
	CRendererDX();
	~CRendererDX();

	HRESULT				Init(HWND hWnd,bool bWindow);
	void				Uninit(void);
	void				Update(void);
	void				Draw(void);
	LPDIRECT3DDEVICE9	GetDevice(void);

private:
#ifdef _DEBUG
	void				DrawFPS(void);
#endif

	LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
	
#ifdef _DEBUG
	LPD3DXFONT				m_pFont;		// フォントへのポインタ
#endif
};

#endif