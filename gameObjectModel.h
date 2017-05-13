//--------------------------------------------------------------------------------
//
//�@gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_MODEL_H_
#define _GAMEOBJECT_MODEL_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
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

	CKFVec3						m_vRot;			// ��]
	int							m_nModelID;		// ���f��ID
	CMM::MATERIAL				m_matType;		// �}�e���A��
};

#endif