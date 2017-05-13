//--------------------------------------------------------------------------------
//	ステージエディタ
//　stageEditor.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _STAGE_EDITOR_H_
#define _STAGE_EDITOR_H_

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CStageEditor : public CStage
{
public:
	CStageEditor();
	~CStageEditor();

	HRESULT Init(const CKFVec3 &vPos, const CKFVec3 &vRot);
	void Uninit(void);
	void Update(void);
	void LateUpdate(void);
	void Draw(void);

	static CStageEditor *Create(const CKFVec3 &vPos, const CKFVec3 &vRot);
private:

	void UpdateSelectionBox(void);
	void UpdateChooseBlock(void);
	void DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage);
	void DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage);
	void SaveStage(void);
	void LoadStage(void);

	int					m_nActLayer;
	bool				m_bViewMode;
};

#endif