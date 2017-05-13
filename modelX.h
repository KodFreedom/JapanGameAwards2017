//--------------------------------------------------------------------------------
//
//�@modelX.h
//	Author : Xu Wenjie
//	Date   : 2017-01-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_X_H_
#define _MODEL_X_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CModelX : public CModel
{
public:
	CModelX();
	~CModelX() {}

	HRESULT		Init(const LPCSTR &pXFilePath);
	void		Uninit(void);
	void		Draw(LPDIRECT3DDEVICE9 pDevice);
	void		Draw(LPDIRECT3DDEVICE9 pDevice, const CMM::MATERIAL &matType);
	void		Draw(LPDIRECT3DDEVICE9 pDevice, const CMM::MATERIAL &matType, const int &nTexID);

	static CModelX *Create(const LPCSTR pXFilePath);
private:

	std::vector<LPDIRECT3DTEXTURE9>	m_vectorTexture;	//texture�C���^�[�t�F�[�X
	LPD3DXMESH						m_pMesh;			//���b�V�����C���^�[�t�F�C�X
	LPD3DXBUFFER					m_pBufferMaterial;	//�}�e���A�����@���I������
	DWORD							m_dwNumMaterial;	//���f���̃}�e���A����
};

#endif