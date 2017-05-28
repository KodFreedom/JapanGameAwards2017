//--------------------------------------------------------------------------------
//	�X�e�[�W
//�@stage.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _STAGE_H_
#define _STAGE_H_

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CHeroin;
class CGameObject2D;

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CStage : public CGameObject
{
public:
	//--------------------------------------------------------------------------------
	//  �萔�錾
	//--------------------------------------------------------------------------------
	static const int s_nNumBlockPerAxis = 5;

	//--------------------------------------------------------------------------------
	//  �\���̐錾
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
	//  �֐��錾
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
	//  �\���̐錾
	//--------------------------------------------------------------------------------
	enum BLOCK_STATUS
	{
		BS_NONE,
		BS_CHOOSEN,
		BS_DIS_CHOOSE,	// �I�ׂȂ� �G�f�B�^��p
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
		int				nLayer;		// �G�f�B�^��p
		CKFVec3			vPos;
		CKFVec3			vRot;
		BLOCK_TYPE		type;
		BLOCK_TYPE		typeNext;
		BLOCK_STATUS	status;
		int				nCntStatus;

		bool operator < (const BLOCK &block)const {
			return fZDepth > block.fZDepth;
		}//Z�\�[�g���邽�߂�operator<���I�[�o�[���[�h����
	};

	struct SELECTION_BOX
	{
		bool		bSelected;		//���N���b�N�����t���O
		BLOCK_TYPE	blockType;		//���u����u���b�N�̃^�C�v
		BLOCK*		pChoosenBlock;	//���w���Ă�u���b�N
		CKFVec3N	vnPos;			//�w���Ă�u���b�N��xyz
	};

	struct UNDO_INFO
	{
		SELECTION_BOX	selectionBox;
		BLOCK			aBlock[s_nNumBlockPerAxis][s_nNumBlockPerAxis][s_nNumBlockPerAxis];
	};

	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	virtual void UpdateSelectionBox(void);
	virtual void CheckWhichIsChoosen(void);
	virtual void DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents);
	virtual void DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents);
	virtual void DrawWireBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents);
	
	//--------------------------------------------------------------------------------
	//  �ϐ��錾
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
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const int s_nCntForChange = 10;
	static const int s_nCntWaitChange = 5;
	static const int s_nNumUndoMax = 3;
	static const int s_nNumStageMax = 10;

	//--------------------------------------------------------------------------------
	//  �N���X���̃N���X�錾
	//--------------------------------------------------------------------------------
	class CChangeBlock
	{//���𔒂ɐ��߂�
	public:
		virtual void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange);
	};

	class CKillBlock : public CChangeBlock
	{//���𔭎˂���
	public:
		void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange) override;
	};

	class CVirtualChangeBlock : public CChangeBlock
	{//�e�������u���b�N��\������
	public:
		void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange) override;
	};

	class CNullChangeBlock : public CChangeBlock
	{//�������Ȃ�
	public:
		void ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange) override {}
	};

	//--------------------------------------------------------------------------------
	//  �֐��錾
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
	//  �ϐ��錾
	//--------------------------------------------------------------------------------
	CChangeBlock		m_changeBlock;
	CKillBlock			m_killBlock;
	CVirtualChangeBlock	m_virtualChangeBlock;
	CNullChangeBlock	m_nullChangeBlock;

	//UI(UI�V�X�e���܂�����ĂȂ����߂����Ő錾����)
	CGameObject2D*				m_pClearUI;
	CGameObject2D*				m_pBlockTypeUI;
	CGameObject2D*				m_pUndoUI;
	CGameObject2D*				m_apStageUI[s_nNumStageMax];
	std::list<CGameObject2D*>	m_listUndoUI;
};

#endif