//--------------------------------------------------------------------------------
//	ステージ
//　stage.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _STAGE_H_
#define _STAGE_H_

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CStage : public CGameObject
{
public:
	CStage();
	~CStage();

	virtual HRESULT Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const LPSTR &stagePath);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void Draw(void);

	static CStage *Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const LPSTR &stagePath);

public:
	enum BLOCK_TYPE
	{
		BT_NONE,
		BT_WHITE,
		BT_BLACK,
		BT_STAIRS,
		BT_GOAL,
		BT_MAX
	};

	enum BLOCK_STATUS
	{
		BS_NONE,
		BS_CHOOSEN,
		BS_DIS_CHOOSE,	// 選べない エディタ専用
		BS_MAX
	};

	struct BLOCK
	{
		float			fZDepth;
		int				nLayer;
		CKFVec3			vPos;
		CKFVec3			vRot;
		BLOCK_TYPE		type;
		BLOCK_STATUS	status;

		bool operator < (const BLOCK &block)const {
			return fZDepth > block.fZDepth;
		}//Zソートするためにoperator<をオーバーロードする
	};

	struct SELECTION_BOX
	{
		bool	bSelected;		//エディタ専用
		BLOCK*	pChoosenBlock;
	};

	static const int m_nNumBlockPerAxis = 5;

	virtual void UpdateSelectionBox(void);
	virtual void UpdateChooseBlock(void);
	virtual void DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage);
	virtual void DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage);

	std::list<BLOCK>	m_listBlock;
	CKFVec3				m_vRot;
	SELECTION_BOX		m_selectionBox;

private:
	void LoadStage(const LPSTR &stagePath);

};

#endif