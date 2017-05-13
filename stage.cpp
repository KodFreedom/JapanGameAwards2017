//--------------------------------------------------------------------------------
//	�X�e�[�W
//�@stage.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "materialManager.h"
#include "textureManager.h"
#include "rendererDX.h"
#include "inputDX.h"
#include "camera.h"
#include "mode.h"
#include "modelManager.h"
#include "model.h"
#include "gameObject.h"
#include "stage.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CStage::CStage() : CGameObject()
	, m_vRot(0.0f)
{
	m_selectionBox.bSelected = false;
	m_selectionBox.pChoosenBlock = NULL;
	m_listBlock.clear();
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CStage::~CStage()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
HRESULT CStage::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const LPSTR &stagePath)
{
	m_vPos = vPos;
	m_vRot = vRot;

	LoadStage(stagePath);

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CStage::Uninit(void)
{
	m_listBlock.clear();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CStage::Update(void)
{
	CKeyboardDX *pKeyboard = GetManager()->GetKeyboard();

	//�}�E�X���C�ƃu���b�N�̓����蔻��X�V
	UpdateChooseBlock();

	//�X�e�[�^�X�X�V
	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		itr->status = BS_NONE;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CStage::LateUpdate(void)
{
	CCamera *pCamera = GetManager()->GetModeNow()->GetCamera();
	CKFVec3 vCameraEye = pCamera->GetPosEye();
	CKFVec3 vCameraAt = pCamera->GetPosAt();

	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		//Z�[�x�X�V
		itr->fZDepth = CKFMath::CalculateZDepth(itr->vPos, vCameraEye, vCameraAt);
	}

	//Z�\�[�g
	m_listBlock.sort();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CStage::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//�X�e�[�W�̍s��Z�o
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//��](Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);

	//���s�ړ�
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);

	//Selection Box�̕`��
	DrawSelectionBox(pDevice, mtxRot, mtxPos);

	//�u���b�N�̕`��
	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		DrawBlock(*itr, pDevice, mtxRot, mtxPos);
	}
}

//--------------------------------------------------------------------------------
//  �}�E�X���C�ƃu���b�N�̓����蔻��X�V
//--------------------------------------------------------------------------------
void CStage::UpdateSelectionBox(void)
{

}

//--------------------------------------------------------------------------------
//  �}�E�X���C�ƃu���b�N�̓����蔻��X�V
//--------------------------------------------------------------------------------
void CStage::UpdateChooseBlock(void)
{
	//���C�Z�o
	CMouseDX *pMouse = GetManager()->GetMouse();
	POINT pMousePos = pMouse->GetMousePos();
	CCamera *pCamera = GetManager()->GetModeNow()->GetCamera();
	D3DXMATRIX mtx = pCamera->GetMtxViewInverse();
	CKFRay ray = CKFMath::CalculatePickingRay(CKFVec2((float)pMousePos.x, (float)pMousePos.y), mtx);

	//�v�Z�p�ϐ�
	CKFMath::RTS_INFO rtsMin;
	rtsMin.bIsContact = false;
	rtsMin.fTimingMin = 0.0f;
	m_selectionBox.pChoosenBlock = NULL;

	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		//���C�g�̓����蔻��
		//��������ԋ߂��̃u���b�N��I��
		CKFMath::RTS_INFO rts = CKFMath::ContactRaytoSphere(ray, itr->vPos, 0.5f);
		if (rts.bIsContact)
		{
			if (m_selectionBox.pChoosenBlock == NULL)
			{
				m_selectionBox.pChoosenBlock = &(*itr);
				rtsMin = rts;
			}
			else
			{
				if (rts.fTimingMin < rtsMin.fTimingMin)
				{
					m_selectionBox.pChoosenBlock = &(*itr);
					rtsMin = rts;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  �u���b�N�`��
//--------------------------------------------------------------------------------
void CStage::DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage)
{
	CModel *pModel = NULL;
	CMM::MATERIAL mat = CMM::MAT_NORMAL;
	CTM::DEMO_TEX_NAME texture = CTM::DEMO_TEX_MAX;

	//���[���h���΍��W
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&mtxWorld);

	//��](Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, block.vRot.m_fY, block.vRot.m_fX, block.vRot.m_fZ);
	mtxWorld *= mtxRot;

	//���s�ړ�
	D3DXMatrixTranslation(&mtxPos, block.vPos.m_fX, block.vPos.m_fY, block.vPos.m_fZ);
	mtxWorld *= mtxPos;

	//�X�e�[�W�s��ɂ�����
	mtxWorld *= mtxRotStage;
	mtxWorld *= mtxPosStage;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�`��
	switch (block.type)
	{
	case BT_NONE:
		return;
	case BT_WHITE:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_CUBE);
		mat = CMM::MAT_WHITE;
		break;
	case BT_BLACK:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_CUBE);
		mat = CMM::MAT_BLACK;
		break;
	case BT_STAIRS:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_STAIRS);
		mat = CMM::MAT_GRAY;
		break;
	case BT_GOAL:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_GOAL);
		mat = CMM::MAT_GOLDEN;
		break;
	default:
		return;
	}

	switch (block.status)
	{
	case BS_NONE:
		pModel->Draw(pDevice, mat);
		break;
	case BS_DIS_CHOOSE:
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  Selection Box�`��
//--------------------------------------------------------------------------------
void CStage::DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxRotStage, const D3DXMATRIX &mtxPosStage)
{
	if (m_selectionBox.pChoosenBlock == NULL) { return; }

	//���[���h���΍��W
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxPos;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&mtxWorld);

	//���s�ړ�
	D3DXMatrixTranslation(&mtxPos, 
		m_selectionBox.pChoosenBlock->vPos.m_fX, 
		m_selectionBox.pChoosenBlock->vPos.m_fY, 
		m_selectionBox.pChoosenBlock->vPos.m_fZ);
	mtxWorld *= mtxPos;

	//�X�e�[�W�s��ɂ�����
	mtxWorld *= mtxRotStage;
	mtxWorld *= mtxPosStage;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	CModel *pModel = GetManager()->GetModelManager()->GetModel(CModelManager::DEMO_MODEL_SELECTION_BOX);
	CMM::MATERIAL mat = m_selectionBox.bSelected ? CMM::MAT_RED_CHOOSEN : CMM::MAT_RED;
	pModel->Draw(pDevice, mat);
}

//--------------------------------------------------------------------------------
//  �X�e�[�W�̓ǂݍ���
//--------------------------------------------------------------------------------
void CStage::LoadStage(const LPSTR &stagePath)
{
	FILE *pFile;

	//bin file
	fopen_s(&pFile, stagePath, "rb");

	if (pFile != NULL)
	{
		int nNumBlock = m_listBlock.size();
		m_listBlock.clear();

		BLOCK block;
		for (int nCnt = 0; nCnt < nNumBlock; nCnt++)
		{
			fread(&block, sizeof(BLOCK), 1, pFile);
			m_listBlock.push_back(block);
		}

		fclose(pFile);
	}
}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
CStage *CStage::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const LPSTR &stagePath)
{
	CStage *pStage = NULL;
	pStage = new CStage;
	pStage->Init(vPos, vRot, stagePath);
	pStage->m_pri = CMode::PRI_3D;
	pStage->m_nID = GetManager()->GetModeNow()->SaveGameObj(CMode::PRI_3D, pStage);
	return pStage;
}