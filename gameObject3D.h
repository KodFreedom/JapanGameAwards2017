//--------------------------------------------------------------------------------
//
//�@gameObject3D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_3D_H_
#define _GAMEOBJECT_3D_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObject3D : public CGameObject
{
public:
	CGameObject3D();
	~CGameObject3D();

	virtual HRESULT	Init(const int &nVtxNum, const int &nIdxNum, const int &nPolygonNum);
	virtual void	Uninit(void);
	virtual void	Update(void);
	virtual void	LateUpdate(void);
	virtual void	Draw(void);

protected:
	virtual void	SetMatrix(LPDIRECT3DDEVICE9 pDevice);
	virtual void	SetRenderState(LPDIRECT3DDEVICE9 pDevice);
	virtual void	ResetRenderState(LPDIRECT3DDEVICE9 pDevice);

	CKFVec3						m_vRot;			// ��]
	CKFVec3						m_vScale;		// �g�嗦
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuffer;	// �C���f�b�N�X�ւ̃|�C���^
	int							m_nTexID;		// �e�N�X�`��
	CMM::MATERIAL				m_matType;		// �}�e���A��
private:
	HRESULT	CreateBuffer(void);
	
	int		m_nVtxNum;		// ���_��
	int		m_nIdxNum;		// �C���f�b�N�X��
	int		m_nPolygonNum;	// �|���S����
};

#endif