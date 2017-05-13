//--------------------------------------------------------------------------------
//
//　gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_MODEL_H_
#define _GAMEOBJECT_MODEL_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectModel : public CGameObject
{
public:
	CGameObjectModel();
	~CGameObjectModel();

	HRESULT	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nModelID);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Draw(void);

	static CGameObjectModel *Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nModelID);
protected:
	void	SetMatrix(LPDIRECT3DDEVICE9 pDevice);
	void	SetRenderState(LPDIRECT3DDEVICE9 pDevice);
	void	ResetRenderState(LPDIRECT3DDEVICE9 pDevice);

	CKFVec3						m_vRot;			// 回転
	int							m_nModelID;		// モデルID
	CMM::MATERIAL				m_matType;		// マテリアル
};

#endif