//--------------------------------------------------------------------------------
//
//　modelX.h
//	Author : Xu Wenjie
//	Date   : 2017-01-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_X_H_
#define _MODEL_X_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
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

	std::vector<LPDIRECT3DTEXTURE9>	m_vectorTexture;	//textureインターフェース
	LPD3DXMESH						m_pMesh;			//メッシュ情報インターフェイス
	LPD3DXBUFFER					m_pBufferMaterial;	//マテリアル情報　動的メモリ
	DWORD							m_dwNumMaterial;	//モデルのマテリアル数
};

#endif