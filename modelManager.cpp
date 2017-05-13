//--------------------------------------------------------------------------------
//
//�@modelManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "modelManager.h"
#include "model.h"
#include "modelX.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
CModelManager::MODEL_INFO CModelManager::m_apDemoModelInfo[DEMO_MODEL_MAX] =
{
	{ CModelManager::XFILE, "data/MODEL/cube.x" },				//cube
	{ CModelManager::XFILE, "data/MODEL/selectionBox.x" },		//selection box
	{ CModelManager::XFILE, "data/MODEL/stairs.x" },			//stairs
	{ CModelManager::XFILE, "data/MODEL/goal.x" },				//goal
};

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModelManager::CModelManager()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CModelManager::Init(void)
{
	m_vectorModel.clear();
}

//--------------------------------------------------------------------------------
//  ���f���̓ǂݍ���
//--------------------------------------------------------------------------------
void CModelManager::LoadAll(const CManager::MODE &mode)
{
	switch (mode)
	{
	case CManager::MODE_DEMO:
		Load(&m_apDemoModelInfo[0], DEMO_MODEL_MAX);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  ���f���̔j��
//--------------------------------------------------------------------------------
void CModelManager::UnloadAll(void)
{
	for (int nCnt = 0; nCnt < (int)m_vectorModel.size(); nCnt++)
	{
		if (m_vectorModel[nCnt] != NULL)
		{
			m_vectorModel[nCnt]->Uninit();
			delete m_vectorModel[nCnt];
			m_vectorModel[nCnt] = NULL;
		}
	}

	m_vectorModel.clear();
}

//--------------------------------------------------------------------------------
//  ���f���̎擾
//--------------------------------------------------------------------------------
CModel *CModelManager::GetModel(const int &nModelID)
{
	if (nModelID < 0 || nModelID >= (int)m_vectorModel.size()) { return NULL; }

	return m_vectorModel[nModelID];
}

//--------------------------------------------------------------------------------
//  ���f���̓ǂݍ���
//--------------------------------------------------------------------------------
void CModelManager::Load(MODEL_INFO *pModelBegin, const int &nModelMax)
{
	for (int nCnt = 0; nCnt < nModelMax; nCnt++)
	{
		CModel *pModel = NULL;
		switch (pModelBegin->type)
		{
		case XFILE:
			pModel = CModelX::Create(pModelBegin->path);
			break;
		case XFILE_MOTION:
			break;
		default:
			break;
		}

		m_vectorModel.push_back(pModel);
		pModelBegin++;
	}
}