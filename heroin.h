//--------------------------------------------------------------------------------
//
//�@heroin.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _HEROIN_H_
#define _HEROIN_H_

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CGameObject2D;

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectModel.h"
#include "KF_Math.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CHeroin : public CGameObjectModel
{
public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	enum STATUS
	{
		HS_UP,
		HS_NORMAL,
		HS_DOWN,
		HS_STAIR_UP,
		HS_STAIR_DOWN,
		HS_NONE
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CHeroin();
	~CHeroin() {}

	KFRESULT	Init(void);
	void		Update(void) override;
	void		LateUpdate(void) override;
	bool		IsClear(void) { return m_bClear; }
	void		SetHeroin(const CKFVec3N &vnPos);

	void	SaveLast(const int &nLimit)
	{
		HEROIN_INFO info;
		info.param = m_param;
		info.vPos = m_vPos;
		info.vRot = m_vRot;

		while ((int)m_listUndo.size() >= nLimit)
		{
			m_listUndo.pop_back();
		}
		m_listUndo.push_front(info);
	}

	void	LoadLast(void)
	{
		if (m_listUndo.size() == 0)
		{
			return;
		}

		std::list<HEROIN_INFO>::iterator itr = m_listUndo.begin();
		m_param = itr->param;
		m_vPos = itr->vPos;
		m_vRot = itr->vRot;
		m_listUndo.pop_front();
	}

	static CHeroin*	Create(void);
private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct HEROIN_PARAM
	{
		CKFVec3			vForward;
		CKFVec3			vForwardNext;
		CKFVec3			vUp;;
		CKFVec3			vUpNext;
		CKFVec3N		vnNextPos;		// 0 �` 6 (�X�e�[�W��1 �` 5)	
		CKFVec3N		vnPos;			// 0 �` 6 (�X�e�[�W��1 �` 5)	
		STATUS			statusNow;
		STATUS			statusNext;
		bool			bChanged;
	};

	struct HEROIN_INFO
	{
		HEROIN_PARAM	param;
		CKFVec3			vPos;
		CKFVec3			vRot;
	};

	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	const float m_fSpeed = 1.0f / 60.0f;
	const float m_fRadius = 0.25f;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void SearchNext(void);
	bool SearchHere(const CKFVec3N &vnPos);
	bool CanSheGomeHere(const CKFVec3N &vnPos);
	void GomeHere(const CKFVec3N &vnPos);
	bool CheckFastForward(POINT pos);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	HEROIN_PARAM			m_param;
	bool					m_bClear;
	std::list<HEROIN_INFO>	m_listUndo;
	CGameObject2D*			m_fastForwardUI;

	CKFVec3 m_avPosOnStage[7][7][7];
};

#endif