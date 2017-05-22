//--------------------------------------------------------------------------------
//
//�@textureManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "manager.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CTM CTextureManager	//�e�N�X�`���}�l�[�W���̗���

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CTextureManager
{
public:
	typedef enum
	{
		TEX_NONE,
		TEX_TEST,
		TEX_POLYGON_ALPHA,
		TEX_ROAD,
		TEX_MAX
	}TEX_NAME;

	CTextureManager();
	~CTextureManager() {}

	void					Init(void);
	void					LoadAll(void);
	void					UnloadAll(void);
	LPDIRECT3DTEXTURE9		GetTexture(const TEX_NAME &texName);
private:
	void Load(const TEX_NAME &texBegin, const TEX_NAME &texEnd);

	std::vector<LPDIRECT3DTEXTURE9>	m_vectorTexture;
	static LPCSTR					m_apTexPath[TEX_MAX];
};

#endif