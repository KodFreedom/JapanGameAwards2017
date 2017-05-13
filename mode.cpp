//--------------------------------------------------------------------------------
//
//�@mode.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CMode::CMode()
	: m_pCamera(NULL)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		m_avectorGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CMode::Uninit(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{

			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->Uninit();
				delete m_avectorGameObj[nCntPri][nCnt];
				m_avectorGameObj[nCntPri][nCnt] = NULL;
			}
		}

		m_avectorGameObj[nCntPri].clear();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CMode::Update(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{
			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->Update();
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CMode::LateUpdate(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{
			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->LateUpdate();
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CMode::Draw(void)
{
	for (int nCntPri = 0; nCntPri < PRI_MAX; nCntPri++)
	{
		for (int nCnt = 0; nCnt < (int)m_avectorGameObj[nCntPri].size(); nCnt++)
		{
			if (m_avectorGameObj[nCntPri][nCnt] != NULL)
			{
				m_avectorGameObj[nCntPri][nCnt]->Draw();
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�̊m��
//--------------------------------------------------------------------------------
int CMode::SaveGameObj(const PRIORITY &pri, CGameObject *pGameObj)
{
	for (int nCnt = 0; nCnt < (int)m_avectorGameObj[pri].size(); nCnt++)
	{
		if (m_avectorGameObj[pri][nCnt] == NULL)
		{
			m_avectorGameObj[pri][nCnt] = pGameObj;
			return nCnt;
		}
	}

	m_avectorGameObj[pri].push_back(pGameObj);
	return (int)m_avectorGameObj[pri].size();
}

//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�̔j��
//--------------------------------------------------------------------------------
void CMode::ReleaseGameObj(const PRIORITY &pri, const int &nID)
{
	PRIORITY priCopy = pri;
	int nIDCopy = nID;

	if (m_avectorGameObj[priCopy][nIDCopy] != NULL)
	{
		delete m_avectorGameObj[priCopy][nIDCopy];
		m_avectorGameObj[priCopy][nIDCopy] = NULL;
	}
}