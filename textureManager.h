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
		DEMO_TEX_TEST,
		DEMO_TEX_POLYGON_ALPHA,
		DEMO_TEX_ROAD,
		DEMO_TEX_MAX
	}DEMO_TEX_NAME;

	CTextureManager();
	~CTextureManager() {}

	void					Init(void);
	void					LoadAll(const CManager::MODE &mode);
	void					UnloadAll(void);
	LPDIRECT3DTEXTURE9		GetTexture(const int &nTexID);
private:
	void Load(LPCSTR *pPathBegin, const int &nTexMax);

	std::vector<LPDIRECT3DTEXTURE9>	m_vectorTexture;
	static LPCSTR					m_apDemoTexPath[DEMO_TEX_MAX];
};

#endif