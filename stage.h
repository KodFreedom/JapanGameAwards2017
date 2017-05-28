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
class CHeroin;
class CGameObject2D;

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CStage : public CGameObject
{
public:
	//--------------------------------------------------------------------------------
	//  定数宣言
	//--------------------------------------------------------------------------------
	static const int s_nNumBlockPerAxis = 5;

	//--------------------------------------------------------------------------------
	//  構造体宣言
	//--------------------------------------------------------------------------------
	enum BLOCK_TYPE
	{
		BT_NONE,
		BT_WHITE,
		BT_BLACK,
		BT_STAIRS,
		BT_GOAL,
		BT_MAX
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	CStage();
	~CStage() {}

	virtual HRESULT		Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nStage);
	virtual void		Uninit(void);
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Draw(void);

	BLOCK_TYPE			GetBlockType(const int &nZ, const int &nY, const int &nX);
	CKFVec3				GetBlockRot(const int &nZ, const int &nY, const int &nX);


	static CStage*		Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nStage);

public:
	//--------------------------------------------------------------------------------
	//  構造体宣言
	//--------------------------------------------------------------------------------
	enum BLOCK_STATUS
	{
		BS_NONE,
		BS_CHOOSEN,
		BS_DIS_CHOOSE,	// 選べない エディタ専用
		BS_CAN_CHOOSE,
		BS_CAN_CHANGE,
		BS_WAIT_CHANGE,
		BS_CHANGING,
		BS_MAX
	};

	struct BLOCK_FOR_LOAD
	{
		float			fZDepth;
		int				nLayer;
		CKFVec3			vPos;
		CKFVec3			vRot;
		BLOCK_TYPE		type;
		BLOCK_STATUS	status;
	};

	struct BLOCK
	{
		float			fZDepth;
		int				nLayer;		// エディタ専用
		CKFVec3			vPos;
		CKFVec3			vRot;
		BLOCK_TYPE		type;
		BLOCK_TYPE		typeNext;
		BLOCK_STATUS	status;
		int				nCntStatus;

		bool operator < (const BLOCK &block)const {
			return fZDepth > block.fZDepth;
		}//Zソートするためにoperator<をオーバーロードする
	};

	struct SELECTION_BOX
	{
		bool		bSelected;		//左クリックしたフラグ
		BLOCK_TYPE	blockType;		//今置けるブロックのタイプ
		BLOCK*		pChoosenBlock;	//今指してるブロック
		CKFVec3N	vnPos;			//指してるブロックのxyz
	};

	struct UNDO_INFO
	{
		SELECTION_BOX	selectionBox;
		BLOCK			aBlock[s_nNumBlockPerAxis][s_nNumBlockPerAxis][s_nNumBlockPerAxis];
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	virtual void UpdateSelectionBox(void);
	virtual void CheckWhichIsChoosen(void);
	virtual void DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents);
	virtual void DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents);
	virtual void DrawWireBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents);
	
	//--------------------------------------------------------------------------------
	//  変数宣言
	//--------------------------------------------------------------------------------
	CKFVec3					m_vRot;
	SELECTION_BOX			m_selectionBox;
	std::list<BLOCK>		m_listBlock;
	BLOCK*					m_apBlock[s_nNumBlockPerAxis][s_nNumBlockPerAxis][s_nNumBlockPerAxis];
	std::list<UNDO_INFO>	m_listUndo;
	int						m_nUndone;
	int						m_nUndoMax;
	bool					m_bDrawWireBox;
	CHeroin*				m_pHeroin;

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const int s_nCntForChange = 10;
	static const int s_nCntWaitChange = 5;
	static const int s_nNumUndoMax = 3;
	static const int s_nNumStageMax = 10;

	//--------------------------------------------------------------------------------
	//  クラス内のクラス宣言
	//--------------------------------------------------------------------------------
	class CChangeBlock
	{//黒を白に染める
	public:
		virtual void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange);
	};

	class CKillBlock : public CChangeBlock
	{//白を発射する
	public:
		void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange) override;
	};

	class CVirtualChangeBlock : public CChangeBlock
	{//影響されるブロックを表示する
	public:
		void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange) override;
	};

	class CNullChangeBlock : public CChangeBlock
	{//何もしない
	public:
		void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange) override {}
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	void CopyZYXSortedStage(void);
	bool FlipLine(const CKFVec3N &vnPos, const BLOCK_TYPE &type, CChangeBlock* pChangeBlock);
	void SetByChangeList(std::list<CKFVec3N> &listChange, const BLOCK_TYPE &checkType, CChangeBlock* pChangeBlock);
	void SaveDoing(void);
	void LoadLastDone(void);
	void LoadStage(const int &nStage);
	void ShootBlock(const int &nZ, const int &nY, const int &nX);

	//UI
	bool CheckUndo(POINT pos);
	int  CheckSelectStage(POINT pos);

	//--------------------------------------------------------------------------------
	//  変数宣言
	//--------------------------------------------------------------------------------
	CChangeBlock		m_changeBlock;
	CKillBlock			m_killBlock;
	CVirtualChangeBlock	m_virtualChangeBlock;
	CNullChangeBlock	m_nullChangeBlock;

	//UI(UIシステムまだ作ってないためここで宣言する)
	CGameObject2D*				m_pClearUI;
	CGameObject2D*				m_pBlockTypeUI;
	CGameObject2D*				m_pUndoUI;
	CGameObject2D*				m_apStageUI[s_nNumStageMax];
	std::list<CGameObject2D*>	m_listUndoUI;
};

#endif