//--------------------------------------------------------------------------------
//
//　gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "mode.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObject
{
public:
	CGameObject() : m_vPos(CKFVec3(0.0f)), m_pVtxBuffer(NULL), m_nID(-1), m_pri(CMode::PRI_MAX) {}
	~CGameObject() {}

	virtual HRESULT Init(void) { return S_OK; }
	virtual void	Uninit(void) = 0;
	virtual void	Update(void) = 0;
	virtual void	LateUpdate(void) = 0;
	virtual void	Draw(void) = 0;

	void			Release(void);
	CKFVec3			GetPos(void);
	void			SetPos(const CKFVec3 &vPos);

protected:
	CKFVec3						m_vPos;			//オブジェクト位置
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffer;	//頂点バッファ管理インターフェースポインタ
	int							m_nID;
	CMode::PRIORITY				m_pri;
};

#endif