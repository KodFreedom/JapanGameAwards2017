//--------------------------------------------------------------------------------
//
//�@gameObject2D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_2D_H_
#define _GAMEOBJECT_2D_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObject2D : public CGameObject
{
public:
	CGameObject2D();
	~CGameObject2D();

	HRESULT Init(const CKFVec3 &vPos, const float &fRot, const CKFVec2 &vSize, const int &nTexID);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Draw(void);

protected:
	virtual void		SetRenderState(LPDIRECT3DDEVICE9 pDevice);
	virtual void		ResetRenderState(LPDIRECT3DDEVICE9 pDevice);

	static const int	m_nNumVtx2D = 4;		//2D�I�u�W�F�N�g�̒��_��
	static const int	m_nNumPolygon2D = 2;	//2D�I�u�W�F�N�g�̃|���S����
	float				m_fRot;					//��]
	CKFVec2				m_vSize;				//�T�C�Y
	CKFColor			m_cColor;				//�J���[

private:
	HRESULT						MakeVertex(void);
	void						UpdateVertex(void);
	void						SetVtxPos(VERTEX_2D *pVtx);
	void						SetVtxColor(VERTEX_2D *pVtx);
	void						SetVtxUV(VERTEX_2D *pVtx);

	int							m_nTexID;		//�e�N�X�`��
};

#endif