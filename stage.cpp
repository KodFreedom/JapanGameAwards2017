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
#include "soundManager.h"
#include "rendererDX.h"
#include "inputDX.h"
#include "camera.h"
#include "mode.h"
#include "modelManager.h"
#include "model.h"
#include "gameObject.h"
#include "gameObject2D.h"
#include "effectCube.h"
#include "stage.h"
#include "heroin.h"

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
	, m_nUndoMax(0)
	, m_nUndone(0)
	, m_bDrawWireBox(true)
	, m_pHeroin(NULL)
	, m_pClearUI(NULL)
	, m_pBlockTypeUI(NULL)
	, m_pUndoUI(NULL)
{
	for (int nCnt = 0; nCnt < s_nNumStageMax; nCnt++)
	{
		m_apStageUI[nCnt] = NULL;
	}
	m_selectionBox.bSelected = false;
	m_selectionBox.pChoosenBlock = NULL;
	m_selectionBox.blockType = BT_NONE;
	m_selectionBox.vnPos = CKFVec3N(0, 0, 0);
	m_listBlock.clear();
	m_listUndo.clear();
	m_listUndoUI.clear();
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
HRESULT CStage::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nStage)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_nUndoMax = s_nNumUndoMax;

	//UI
	m_pUndoUI = CGameObject2D::Create(CKFVec3(SCREEN_WIDTH - 80.0f, SCREEN_HEIGHT * 0.25f, 0.0f), 0.0f, CKFVec2(160.0f, 40.0f), CTM::TEX_UNDO);
	m_pBlockTypeUI = CGameObject2D::Create(CKFVec3(SCREEN_WIDTH * 0.95f, SCREEN_HEIGHT * 0.75f, 0.0f), 0.0f, CKFVec2(80.0f, 80.0f), CTM::TEX_WHITE);
	for (int nCnt = 0; nCnt < s_nNumStageMax; nCnt++)
	{
		m_apStageUI[nCnt] = CGameObject2D::Create(CKFVec3(80.0f, 20.0f + 60.0f * nCnt, 0.0f), 0.0f, CKFVec2(160.0f, 40.0f), (CTM::TEX_NAME)((int)CTM::TEX_STAGE1 + nCnt));
	}

	LoadStage(nStage);

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
	CMouseDX *pMouse = GetManager()->GetMouse();
	POINT pos = pMouse->GetMousePos();

	int nSelectStage = CheckSelectStage(pos);
	if (nSelectStage != -1)
	{
		LoadStage(nSelectStage);
	}

	if (m_selectionBox.bSelected)
	{
		//Selection Box�̍X�V
		UpdateSelectionBox();
	}

	if (m_pHeroin)
	{//check clear
		if (m_pHeroin->IsClear())
		{
			if (m_pClearUI == NULL)
			{
				m_pClearUI = CGameObject2D::Create(CKFVec3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0.0f, CKFVec2(SCREEN_WIDTH, SCREEN_HEIGHT), CTM::TEX_CLEAR);
			}
			return;
		}
	}

	if (pMouse->GetMouseTrigger(CMouseDX::MOUSE_LEFT))
	{
		if (CheckUndo(pos) && m_listUndo.size() > 0 && (m_nUndone < m_nUndoMax))
		{
			LoadLastDone();
			GetManager()->GetSoundManager()->Play(CSM::SE_UICLICK);
			m_nUndone++;
			std::list<CGameObject2D*>::iterator itr = m_listUndoUI.begin();
			(*itr)->Release();
			m_listUndoUI.pop_front();
		}
	}

	//�X�e�[�^�X�X�V
	for (int nCntZ = 0; nCntZ < s_nNumBlockPerAxis; nCntZ++)
	{
		for (int nCntY = 0; nCntY < s_nNumBlockPerAxis; nCntY++)
		{
			for (int nCntX = 0; nCntX < s_nNumBlockPerAxis; nCntX++)
			{
				if ((m_apBlock[nCntZ][nCntY][nCntX]->type == BT_WHITE ||
					m_apBlock[nCntZ][nCntY][nCntX]->type == BT_BLACK) &&
					m_apBlock[nCntZ][nCntY][nCntX]->status != BS_NONE)
				{
					if (m_apBlock[nCntZ][nCntY][nCntX]->nCntStatus != 0)
					{
						m_apBlock[nCntZ][nCntY][nCntX]->nCntStatus--;
						continue;
					}

					if (m_apBlock[nCntZ][nCntY][nCntX]->nCntStatus == 0)
					{
						switch (m_apBlock[nCntZ][nCntY][nCntX]->status)
						{
						case BS_WAIT_CHANGE:
							GetManager()->GetSoundManager()->Play(CSM::SE_COLORCHANGE);
							m_apBlock[nCntZ][nCntY][nCntX]->status = BS_CHANGING;
							m_apBlock[nCntZ][nCntY][nCntX]->nCntStatus = s_nCntForChange;

							if (m_apBlock[nCntZ][nCntY][nCntX]->typeNext == BT_NONE)
							{
								ShootBlock(nCntZ, nCntY, nCntX);
							}
							break;
						case BS_CHANGING:
							m_apBlock[nCntZ][nCntY][nCntX]->type = m_apBlock[nCntZ][nCntY][nCntX]->typeNext;
							m_apBlock[nCntZ][nCntY][nCntX]->status = BS_NONE;
							break;
						default:
							m_apBlock[nCntZ][nCntY][nCntX]->status = BS_NONE;
							break;
						}
					}
				}
				else
				{
					m_apBlock[nCntZ][nCntY][nCntX]->status = BS_NONE;
				}
			}
		}
	}

	//�I���ł���u���b�N�X�V
	for (int nCntZ = 0; nCntZ < s_nNumBlockPerAxis; nCntZ++)
	{
		for (int nCntY = 0; nCntY < s_nNumBlockPerAxis; nCntY++)
		{
			for (int nCntX = 0; nCntX < s_nNumBlockPerAxis; nCntX++)
			{
				if (m_apBlock[nCntZ][nCntY][nCntX]->type == BT_NONE)
				{
					if (FlipLine(CKFVec3N(nCntX, nCntY, nCntZ), m_selectionBox.blockType, &m_nullChangeBlock))
					{
						m_apBlock[nCntZ][nCntY][nCntX]->status = BS_CAN_CHOOSE;
					}
				}
			}
		}
	}

	//�}�E�X���C�ƃu���b�N�̓����蔻��X�V
	CheckWhichIsChoosen();

	m_pBlockTypeUI->SetTexture(m_selectionBox.blockType == BT_WHITE ? CTM::TEX_WHITE : CTM::TEX_BLACK);
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
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	D3DXMatrixIdentity(&mtxWorld);

	//��](Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);
	mtxWorld *= mtxRot;

	//���s�ړ�
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);
	mtxWorld *= mtxPos;

	//�u���b�N�̕`��
	std::list<BLOCK>::iterator itr;
	for (itr = m_listBlock.begin(); itr != m_listBlock.end(); itr++)
	{
		DrawBlock(*itr, pDevice, mtxWorld);
	}

	//Selection Box�̕`��
	DrawSelectionBox(pDevice, mtxWorld);

	//�O�g�̕`��
	DrawWireBox(pDevice, mtxWorld);
}

//--------------------------------------------------------------------------------
//  �}�E�X���C�ƃu���b�N�̓����蔻��X�V
//--------------------------------------------------------------------------------
void CStage::UpdateSelectionBox(void)
{
	m_selectionBox.pChoosenBlock->status = BS_CHOOSEN;      //�u�����u���b�N��I��ł����Ԃɂ���
	m_selectionBox.pChoosenBlock->type = m_selectionBox.blockType;
	m_selectionBox.blockType = m_selectionBox.blockType == BT_WHITE ? BT_BLACK : BT_WHITE;
	m_selectionBox.bSelected = false;

	//���߂�i�S�����j
	if (m_selectionBox.pChoosenBlock->type == BT_WHITE)
	{
		FlipLine(m_selectionBox.vnPos, m_selectionBox.pChoosenBlock->type, &m_changeBlock);
	}
	else
	{
		FlipLine(m_selectionBox.vnPos, m_selectionBox.pChoosenBlock->type, &m_killBlock);
	}
	
}

//--------------------------------------------------------------------------------
//  �}�E�X���C�ƃu���b�N�̓����蔻��X�V
//--------------------------------------------------------------------------------
void CStage::CheckWhichIsChoosen(void)
{
	//���C�Z�o
	CMouseDX *pMouse = GetManager()->GetMouse();
	POINT pMousePos = pMouse->GetMousePos();
	CKFRay ray = CKFUtilityDX::ChangeToPickingRay(CKFVec2((float)pMousePos.x, (float)pMousePos.y));

	//�v�Z�p�ϐ�
	CKFMath::RTS_INFO rtsMin;
	rtsMin.bIsContact = false;
	rtsMin.fTimingMin = 0.0f;
	m_selectionBox.pChoosenBlock = NULL;

	for (int nCntZ = 0; nCntZ < s_nNumBlockPerAxis; nCntZ++)
	{
		for (int nCntY = 0; nCntY < s_nNumBlockPerAxis; nCntY++)
		{
			for (int nCntX = 0; nCntX < s_nNumBlockPerAxis; nCntX++)
			{
				if (m_apBlock[nCntZ][nCntY][nCntX]->type == BT_NONE && m_apBlock[nCntZ][nCntY][nCntX]->status == BS_CAN_CHOOSE)
				{
					CKFMath::RTS_INFO rts = CKFMath::ContactRayToSphere(ray, m_apBlock[nCntZ][nCntY][nCntX]->vPos, 0.5f);
					if (rts.bIsContact)
					{
						CKFVec3N vnPosBlock = CKFVec3N(nCntX, nCntY, nCntZ);
						if (m_apBlock[nCntZ][nCntY][nCntX]->status == BS_CAN_CHOOSE)
						{
							if (m_selectionBox.pChoosenBlock == NULL)
							{
								m_selectionBox.pChoosenBlock = m_apBlock[nCntZ][nCntY][nCntX];
								m_selectionBox.vnPos = vnPosBlock;
								rtsMin = rts;
							}
							else
							{
								if (rts.fTimingMin < rtsMin.fTimingMin)
								{
									m_selectionBox.pChoosenBlock = m_apBlock[nCntZ][nCntY][nCntX];
									m_selectionBox.vnPos = vnPosBlock;
									rtsMin = rts;
								}
							}
						}
					}
				}
			}
		}
	}

	//���N���b�N������I�������u���b�N�����b�N����(������񍶃N���b�N���Ȃ��ƊO���Ȃ�)
	if (m_selectionBox.pChoosenBlock != NULL)
	{
		if (pMouse->GetMouseTrigger(CMouseDX::MOUSE_LEFT))
		{
			if (m_nUndoMax > m_nUndone)
			{
				SaveDoing();
			}

			m_selectionBox.bSelected = true;
		}
		else
		{//�ς�����u���b�N��\��
			FlipLine(m_selectionBox.vnPos, m_selectionBox.blockType, &m_virtualChangeBlock);
		}
	}
}

//--------------------------------------------------------------------------------
//  �u���b�N�`��
//--------------------------------------------------------------------------------
void CStage::DrawBlock(const BLOCK &block, LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents)
{
	CModel *pModel = NULL;
	CMM::MATERIAL mat = CMM::MAT_NORMAL;
	CTM::TEX_NAME texture = CTM::TEX_MAX;

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
	mtxWorld *= mtxParents;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�`��
	switch (block.type)
	{
	case BT_NONE:
		if (block.status == BS_CAN_CHOOSE)
		{
			pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_SELECTION_BOX);
			mat = CMM::MAT_RED;
		}
		else
		{
			return;
		}
		break;
	case BT_WHITE:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_CUBE);
		mat = CMM::MAT_WHITE;
		break;
	case BT_BLACK:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_CUBE);
		mat = CMM::MAT_BLACK;
		break;
	case BT_STAIRS:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_STAIRS);
		mat = CMM::MAT_GRAY;
		break;
	case BT_GOAL:
		pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_GOAL);
		mat = CMM::MAT_GOLDEN;
		break;
	default:
		return;
	}

	switch (block.status)
	{
	case BS_NONE:
		pModel->Draw(mat);
		break;
	case BS_WAIT_CHANGE:
		pModel->Draw(mat);
		break;
	case BS_CAN_CHOOSE:
		texture = CTM::TEX_POLYGON_ALPHA;
		pModel->Draw(mat, texture);
		break;
	case BS_CAN_CHANGE:
		texture = CTM::TEX_POLYGON_ALPHA;
		pModel->Draw(mat, texture);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  Selection Box�`��
//--------------------------------------------------------------------------------
void CStage::DrawSelectionBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents)
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
	mtxWorld *= mtxParents;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//select box
	CModel *pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_SELECTION_BOX);
	CMM::MATERIAL mat = m_selectionBox.bSelected ? CMM::MAT_RED_CHOOSEN : CMM::MAT_RED;
	pModel->Draw(mat);
}

//--------------------------------------------------------------------------------
//  Wire Box�`��
//--------------------------------------------------------------------------------
void CStage::DrawWireBox(LPDIRECT3DDEVICE9 pDevice, const D3DXMATRIX &mtxParents)
{
	pDevice->SetTransform(D3DTS_WORLD, &mtxParents);

	CModel* pModel = GetManager()->GetModelManager()->GetModel(CModelManager::MODEL_WIREBOX);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pModel->Draw();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------------------------------------
//  �X�e�[�W�̓ǂݍ���
//--------------------------------------------------------------------------------
void CStage::LoadStage(const int &nStage)
{
	FILE *pFile;

	LPSTR str[s_nNumStageMax] = {
		"data/STAGE/stage1.bin",
		"data/STAGE/stage2.bin",
		"data/STAGE/stage3.bin",
		"data/STAGE/stage4.bin",
		"data/STAGE/stage5.bin",
		"data/STAGE/stage6.bin",
		"data/STAGE/stage7.bin",
		"data/STAGE/stage8.bin",
		"data/STAGE/stage9.bin",
		"data/STAGE/stage10.bin"
	};

	//bin file
	fopen_s(&pFile, str[nStage], "rb");

	if (pFile != NULL)
	{
		int nNumBlock = s_nNumBlockPerAxis * s_nNumBlockPerAxis * s_nNumBlockPerAxis;
		m_listBlock.clear();

		BLOCK_FOR_LOAD blockLoad;
		BLOCK block;
		for (int nCnt = 0; nCnt < nNumBlock; nCnt++)
		{
			fread(&blockLoad, sizeof(BLOCK_FOR_LOAD), 1, pFile);

			block.fZDepth = 0.0f;
			block.nCntStatus = 0;
			block.nLayer = blockLoad.nLayer;
			block.status = BS_NONE;
			block.type = blockLoad.type;
			block.typeNext = blockLoad.type;
			block.vPos = blockLoad.vPos;
			block.vRot = blockLoad.vRot;

			m_listBlock.push_back(block);
		}

		CopyZYXSortedStage();
		fclose(pFile);
	}

	m_selectionBox.bSelected = false;
	m_selectionBox.pChoosenBlock = NULL;

	if ((nStage > 3 && nStage < 7) || nStage == 8)
	{
		m_selectionBox.blockType = BT_BLACK;
	}
	else
	{
		m_selectionBox.blockType = BT_WHITE;
	}

	m_listUndo.clear();
	m_nUndone = 0;

	CKFVec3N aPos[s_nNumStageMax] =
	{
		CKFVec3N(4, 2, 1),
		CKFVec3N(2, 1, 2),
		CKFVec3N(1, 2, 1),
		CKFVec3N(5, 2, 5),
		CKFVec3N(1, 2, 4),
		CKFVec3N(1, 2, 3),
		CKFVec3N(5, 1, 5),
		CKFVec3N(1, 3, 5),
		CKFVec3N(1, 3, 5),
		CKFVec3N(5, 4, 1)
	};

	if (m_pHeroin == NULL)
	{
		m_pHeroin = CHeroin::Create();
	}
	m_pHeroin->SetHeroin(aPos[nStage]);

	//ui
	std::list<CGameObject2D*>::iterator itr;
	for (itr = m_listUndoUI.begin(); itr != m_listUndoUI.end(); itr++)
	{
		(*itr)->Release();
	}
	m_listUndoUI.clear();

	for (int nCnt = 0; nCnt < m_nUndoMax; nCnt++)
	{
		CGameObject2D* pObj = CGameObject2D::Create(CKFVec3(SCREEN_WIDTH * 0.95f - 40.0f * nCnt, SCREEN_HEIGHT * 0.25f + 40.0f, 0.0f), 0.0f, CKFVec2(30.0f, 30.0f), CTM::TEX_WHITE);
		m_listUndoUI.push_front(pObj);
	}

	if (m_pClearUI != NULL)
	{
		m_pClearUI->Release();
		m_pClearUI = NULL;
	}
}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
CStage *CStage::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nStage)
{
	CStage *pStage = NULL;
	pStage = new CStage;
	pStage->Init(vPos, vRot, nStage);
	pStage->m_pri = GOM::PRI_3D_ALPHA;
	pStage->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(pStage->m_pri, pStage);
	return pStage;
}

//--------------------------------------------------------------------------------
//  Z-Y-X�̎O�����z��Ƀu���b�N�̃|�C���^�����Ԃɕۑ�����
//--------------------------------------------------------------------------------
void CStage::CopyZYXSortedStage(void)
{
	std::list<BLOCK>::iterator itr = m_listBlock.begin();
	for (int nCntZ = 0; nCntZ < s_nNumBlockPerAxis; nCntZ++)
	{
		for (int nCntY = 0; nCntY < s_nNumBlockPerAxis; nCntY++)
		{
			for (int nCntX = 0; nCntX < s_nNumBlockPerAxis; nCntX++)
			{
				m_apBlock[nCntZ][nCntY][nCntX] = &(*itr);
				itr++;
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  ����X�V�O�Ƀf�[�^���o�b�N�A�b�v����
//--------------------------------------------------------------------------------
void CStage::SaveDoing(void)
{
	UNDO_INFO info;

	for (int nCntZ = 0; nCntZ < s_nNumBlockPerAxis; nCntZ++)
	{
		for (int nCntY = 0; nCntY < s_nNumBlockPerAxis; nCntY++)
		{
			for (int nCntX = 0; nCntX < s_nNumBlockPerAxis; nCntX++)
			{
				info.aBlock[nCntZ][nCntY][nCntX] = *m_apBlock[nCntZ][nCntY][nCntX];
			}
		}
	}

	info.selectionBox = m_selectionBox;

	//�ۑ����������`�F�b�N�A���E�𒴂������ԌÂ��f�[�^������
	while ((int)m_listUndo.size() >= (m_nUndoMax - m_nUndone))
	{
		m_listUndo.pop_back();
	}
	m_listUndo.push_front(info);
	m_pHeroin->SaveLast(m_nUndoMax - m_nUndone);
}

//--------------------------------------------------------------------------------
//  �O��̑����߂�
//--------------------------------------------------------------------------------
void CStage::LoadLastDone(void)
{
	std::list<UNDO_INFO>::iterator itr = m_listUndo.begin();

	for (int nCntZ = 0; nCntZ < s_nNumBlockPerAxis; nCntZ++)
	{
		for (int nCntY = 0; nCntY < s_nNumBlockPerAxis; nCntY++)
		{
			for (int nCntX = 0; nCntX < s_nNumBlockPerAxis; nCntX++)
			{
				*m_apBlock[nCntZ][nCntY][nCntX] = itr->aBlock[nCntZ][nCntY][nCntX];
			}
		}
	}

	m_selectionBox = itr->selectionBox;
	m_pHeroin->LoadLast();
	m_listUndo.pop_front();
}

//--------------------------------------------------------------------------------
//  �u���b�N�̐F��ς���
//--------------------------------------------------------------------------------
void CStage::CChangeBlock::ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange)
{
	pBlock->typeNext = type;
	pBlock->status = BS_WAIT_CHANGE;
	pBlock->nCntStatus = nNoToChange * s_nCntWaitChange;
	}

//--------------------------------------------------------------------------------
//  �u���b�N�̏���
//--------------------------------------------------------------------------------
void CStage::CKillBlock::ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange)
{
	pBlock->typeNext = BT_NONE;
	pBlock->status = BS_WAIT_CHANGE;
	pBlock->nCntStatus = nNoToChange * s_nCntWaitChange;
}

//--------------------------------------------------------------------------------
//  �u���b�N�̕ς�����̕\��
//--------------------------------------------------------------------------------
void CStage::CVirtualChangeBlock::ChangBlock(BLOCK* pBlock, const CKFVec3N &vnPos, const BLOCK_TYPE &type, const int &nNoToChange)
{
	if (pBlock->status == BS_NONE)
	{
		pBlock->status = BS_CAN_CHANGE;
	}
}

//--------------------------------------------------------------------------------
//  �ύX����u���b�N�𗼑�����ϊ����Ă���
//--------------------------------------------------------------------------------
void CStage::SetByChangeList(std::list<CKFVec3N> &listChange, const BLOCK_TYPE &checkType, CChangeBlock* pChangeBlock)
{
	if (!listChange.empty())
	{
		int nNoToChange = 0;
		std::list<CKFVec3N>::iterator itrBegin = listChange.begin();
		std::list<CKFVec3N>::iterator itrEnd = listChange.end();
		
		for (int nCnt = 0; nCnt < (listChange.size() + 1) / 2; nCnt++)
		{
			itrEnd--;
			pChangeBlock->ChangBlock(m_apBlock[itrBegin->m_nZ][itrBegin->m_nY][itrBegin->m_nX], *itrBegin, checkType, nNoToChange);
			pChangeBlock->ChangBlock(m_apBlock[itrEnd->m_nZ][itrEnd->m_nY][itrEnd->m_nX], *itrEnd, checkType, nNoToChange);
			nNoToChange++;
			itrBegin++;
		}
	}
}

//--------------------------------------------------------------------------------
//  �ύX����u���b�N�𗼑�����ϊ����Ă���
//--------------------------------------------------------------------------------
bool CStage::CheckUndo(POINT pos)
{
	CKFVec3 vPosUndo = CKFVec3(SCREEN_WIDTH - 80.0f, SCREEN_HEIGHT * 0.25f, 0.0f);
	CKFVec3 vPosMouse = CKFVec3((float)pos.x, (float)pos.y, 0.0f);
	if (vPosMouse.m_fX >= vPosUndo.m_fX - 80.0f && vPosMouse.m_fX <= vPosUndo.m_fX + 80.0f
		&& vPosMouse.m_fY >= vPosUndo.m_fY - 20.0f && vPosMouse.m_fY <= vPosUndo.m_fY + 20.0f)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------
//  �X�e�[�Wui�Ƃ̓����蔻��
//--------------------------------------------------------------------------------
int CStage::CheckSelectStage(POINT pos)
{
	CMouseDX *pMouse = GetManager()->GetMouse();

	if (pMouse->GetMouseTrigger(CMouseDX::MOUSE_LEFT))
	{
		CKFVec3 vPosMouse = CKFVec3((float)pos.x, (float)pos.y, 0.0f);

		for (int nCnt = 0; nCnt < s_nNumStageMax; nCnt++)
		{
			CKFVec3 vPosUI = CKFVec3(80.0f, 20.0f + 60.0f * nCnt, 0.0f);
			if (vPosMouse.m_fX >= vPosUI.m_fX - 80.0f && vPosMouse.m_fX <= vPosUI.m_fX + 80.0f
				&& vPosMouse.m_fY >= vPosUI.m_fY - 20.0f && vPosMouse.m_fY <= vPosUI.m_fY + 20.0f)
			{
				return nCnt;
			}
		}
	}

	return -1;
}

//--------------------------------------------------------------------------------
//  �{���b�N�𔭎�
//--------------------------------------------------------------------------------
void CStage::ShootBlock(const int &nZ, const int &nY, const int &nX)
{
	CKFVec3 vPos = m_apBlock[nZ][nY][nX]->vPos;
	CKFVec3 vRot = m_apBlock[nZ][nY][nX]->vRot;
	CEffectCube::CUBE_TYPE type = (CEffectCube::CUBE_TYPE)(m_apBlock[nZ][nY][nX]->type - 1);
	CEffectCube::CUBE_DIR dir = CEffectCube::CD_YM;
	m_apBlock[nZ][nY][nX]->type = BT_NONE;

	if (nZ == s_nNumBlockPerAxis / 2
		&& nY == s_nNumBlockPerAxis / 2
		&& nZ == s_nNumBlockPerAxis / 2)
	{
		dir = CEffectCube::CD_YM;

	}

	else if (nZ < s_nNumBlockPerAxis / 2)
	{
		int nResultZ = 2 - nZ;
		int nResultX = abs(nX - 2);
		int nResultY = abs(nY - 2);

		if (nResultX <= nResultZ
			&& nResultY <= nResultZ)
		{
			dir = CEffectCube::CD_ZM;
		}
	}

	else if (nZ > s_nNumBlockPerAxis / 2)
	{
		int nResultZ = nZ - 2;
		int nResultX = abs(nX - 2);
		int nResultY = abs(nY - 2);

		if (nResultX <= nResultZ
			&& nResultY <= nResultZ)
		{
			dir = CEffectCube::CD_ZP;
		}
	}

	else if (nX < s_nNumBlockPerAxis / 2)
	{
		int nResultX = 2 - nX;
		int nResultZ = abs(nZ - 2);
		int nResultY = abs(nY - 2);

		if (nResultZ <= nResultX
			&& nResultY <= nResultX)
		{
			dir = CEffectCube::CD_XM;
		}
	}

	else if (nX > s_nNumBlockPerAxis / 2)
	{
		int nResultX = nX - 2;
		int nResultZ = abs(nZ - 2);
		int nResultY = abs(nY - 2);

		if (nResultZ <= nResultX
			&& nResultY <= nResultX)
		{
			dir = CEffectCube::CD_XP;
		}
	}

	else if (nY < s_nNumBlockPerAxis / 2)
	{
		int nResultY = 2 - nY;
		int nResultZ = abs(nZ - 2);
		int nResultX = abs(nX - 2);

		if (nResultZ <= nResultY
			&& nResultX <= nResultY)
		{
			dir = CEffectCube::CD_YM;
		}
	}

	else if (nY > s_nNumBlockPerAxis / 2)
	{
		int nResultY = nY - 2;
		int nResultZ = abs(nZ - 2);
		int nResultX = abs(nX - 2);

		if (nResultZ <= nResultY
			&& nResultX <= nResultY)
		{
			dir = CEffectCube::CD_YP;
		}
	}

	GetManager()->GetSoundManager()->Play(CSM::SE_COLORCHANGE);
	CEffectCube::Create(vPos, vRot, dir, type);
}

//--------------------------------------------------------------------------------
//  �S�����`�F�b�N����
//--------------------------------------------------------------------------------
bool CStage::FlipLine(const CKFVec3N &vnPos, const BLOCK_TYPE &type, CChangeBlock* pChangeBlock)
{
	int nLine;
	bool bCanChoose = false;
	std::list<CKFVec3N> listChange;
	BLOCK_TYPE checkType = type;
	BLOCK_TYPE blockType = type == BT_WHITE ? BT_BLACK : BT_WHITE;

	//�E����
	listChange.clear();
	if (vnPos.m_nX < s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX + 1;
		while (m_apBlock[vnPos.m_nZ][vnPos.m_nY][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1)
		{//�`�F�b�N����u���b�N�������Ȃ���I���܂ł��
			nLine++;
		}
		if (m_apBlock[vnPos.m_nZ][vnPos.m_nY][nLine]->type == checkType)
		{//�`�F�b�N����u���b�N���I������悪���΂̃^�C�v�̏ꍇ
			nLine--;
			while (nLine > vnPos.m_nX)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, vnPos.m_nY, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//������
	listChange.clear();
	if (vnPos.m_nX >= 1)
	{
		nLine = vnPos.m_nX - 1;
		while (m_apBlock[vnPos.m_nZ][vnPos.m_nY][nLine]->type == blockType
			&& nLine >= 1)
		{
			nLine--;
		}
		if (m_apBlock[vnPos.m_nZ][vnPos.m_nY][nLine]->type == checkType)
		{
			nLine++;
			while (nLine < vnPos.m_nX)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, vnPos.m_nY, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�����
	listChange.clear();
	if (vnPos.m_nY < s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nY + 1;
		while (m_apBlock[vnPos.m_nZ][nLine][vnPos.m_nX]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1)                 //���u���b�N�������Ȃ���I���܂ł��
		{
			nLine++;
		}
		if (m_apBlock[vnPos.m_nZ][nLine][vnPos.m_nX]->type == checkType)      //���u���b�N���I������悪���̏ꍇ
		{
			nLine--;
			while (nLine > vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, nLine, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//������
	listChange.clear();
	if (vnPos.m_nY >= 1)
	{
		nLine = vnPos.m_nY - 1;
		while (m_apBlock[vnPos.m_nZ][nLine][vnPos.m_nX]->type == blockType
			&& nLine >= 1)
		{
			nLine--;
		}
		if (m_apBlock[vnPos.m_nZ][nLine][vnPos.m_nX]->type == checkType)
		{
			nLine++;
			while (nLine < vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, nLine, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//������
	listChange.clear();
	if (vnPos.m_nZ < s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nZ + 1;
		while (m_apBlock[nLine][vnPos.m_nY][vnPos.m_nX]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1)
		{
			nLine++;
		}
		if (m_apBlock[nLine][vnPos.m_nY][vnPos.m_nX]->type == checkType)
		{
			nLine--;
			while (nLine > vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, vnPos.m_nY, nLine);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//��O����
	listChange.clear();
	if (vnPos.m_nZ >= 1)
	{
		nLine = vnPos.m_nZ - 1;
		while (m_apBlock[nLine][vnPos.m_nY][vnPos.m_nX]->type == blockType
			&& nLine >= 1)
		{
			nLine--;
		}
		if (m_apBlock[nLine][vnPos.m_nY][vnPos.m_nX]->type == checkType)
		{
			nLine++;
			while (nLine < vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, vnPos.m_nY, nLine);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	int nLine2;

	//�E�����
	listChange.clear();
	if (vnPos.m_nX< s_nNumBlockPerAxis - 1 && vnPos.m_nY< s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nY + 1;
		while (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 < s_nNumBlockPerAxis - 1)
		{
			nLine++;
			nLine2++;
		}
		if (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == checkType)
		{
			nLine--;
			nLine2--;
			while (nLine > vnPos.m_nX && nLine2 > vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine2, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//��������
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nY >= 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nY - 1;
		while (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == blockType
			&& nLine >= 1
			&& nLine2 >= 1)
		{
			nLine--;
			nLine2--;
		}
		if (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == checkType)
		{
			nLine++;
			nLine2++;
			while (nLine < vnPos.m_nX && nLine2 < vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine2, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�������
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nY< s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nY + 1;
		while (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == blockType
			&& nLine >= 1
			&& nLine2 < s_nNumBlockPerAxis - 1)
		{
			nLine--;
			nLine2++;
		}
		if (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == checkType)
		{
			nLine++;
			nLine2--;
			while (nLine < vnPos.m_nX && nLine2 > vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine2, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�E������
	listChange.clear();
	if (vnPos.m_nX< s_nNumBlockPerAxis - 1 && vnPos.m_nY >= 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nY - 1;
		while (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 >= 1)
		{
			nLine++;
			nLine2--;
		}
		if (m_apBlock[vnPos.m_nZ][nLine2][nLine]->type == checkType)
		{
			nLine--;
			nLine2++;
			while (nLine > vnPos.m_nX && nLine2 < vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine2, vnPos.m_nZ);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�E������
	listChange.clear();
	if (vnPos.m_nX < s_nNumBlockPerAxis - 1 && vnPos.m_nZ < s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nZ + 1;
		while (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 < s_nNumBlockPerAxis - 1)
		{
			nLine++;
			nLine2++;
		}
		if (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == checkType)
		{
			nLine--;
			nLine2--;
			while (nLine > vnPos.m_nX && nLine2 > vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, vnPos.m_nY, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//����O����
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nZ >= 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nZ - 1;
		while (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == blockType
			&& nLine >= 1
			&& nLine2 >= 1)
		{
			nLine--;
			nLine2--;
		}
		if (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == checkType)
		{
			nLine++;
			nLine2++;
			while (nLine < vnPos.m_nX && nLine2 < vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, vnPos.m_nY, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//��������
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nZ < s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nZ + 1;
		while (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == blockType
			&& nLine >= 1
			&& nLine2 < s_nNumBlockPerAxis - 1)
		{
			nLine--;
			nLine2++;
		}
		if (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == checkType)
		{
			nLine++;
			nLine2--;
			while (nLine < vnPos.m_nX && nLine2 > vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, vnPos.m_nY, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�E��O����
	listChange.clear();
	if (vnPos.m_nX < s_nNumBlockPerAxis - 1 && vnPos.m_nZ >= 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nZ - 1;
		while (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 >= 1)
		{
			nLine++;
			nLine2--;
		}
		if (m_apBlock[nLine2][vnPos.m_nY][nLine]->type == checkType)
		{
			nLine--;
			nLine2++;
			while (nLine > vnPos.m_nX && nLine2 < vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, vnPos.m_nY, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�㉜����
	listChange.clear();
	if (vnPos.m_nZ < s_nNumBlockPerAxis - 1 && vnPos.m_nY < s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nZ + 1;
		nLine2 = vnPos.m_nY + 1;
		while (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 < s_nNumBlockPerAxis - 1)
		{
			nLine++;
			nLine2++;
		}
		if (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == checkType)
		{
			nLine--;
			nLine2--;
			while (nLine > vnPos.m_nZ && nLine2 > vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, nLine2, nLine);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//����O����
	listChange.clear();
	if (vnPos.m_nZ >= 1 && vnPos.m_nY >= 1)
	{
		nLine = vnPos.m_nZ - 1;
		nLine2 = vnPos.m_nY - 1;
		while (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == blockType
			&& nLine >= 1
			&& nLine2 >= 1)
		{
			nLine--;
			nLine2--;
		}
		if (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == checkType)
		{
			nLine++;
			nLine2++;
			while (nLine < vnPos.m_nZ && nLine2 < vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, nLine2, nLine);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//���O����
	listChange.clear();
	if (vnPos.m_nZ >= 1 && vnPos.m_nY< s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nZ - 1;
		nLine2 = vnPos.m_nY + 1;
		while (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == blockType
			&& nLine >= 1
			&& nLine2 < s_nNumBlockPerAxis - 1)             //���u���b�N�������Ȃ���I���܂ł��
		{
			nLine--;
			nLine2++;
		}
		if (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == checkType)      //���u���b�N���I������悪���̏ꍇ
		{
			nLine++;
			nLine2--;
			while (nLine < vnPos.m_nZ && nLine2 > vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, nLine2, nLine);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//��������
	listChange.clear();
	if (vnPos.m_nZ< s_nNumBlockPerAxis - 1 && vnPos.m_nY >= 1)
	{
		nLine = vnPos.m_nZ + 1;
		nLine2 = vnPos.m_nY - 1;
		while (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 >= 1)                                     //���u���b�N�������Ȃ���I���܂ł��
		{
			nLine++;
			nLine2--;
		}
		if (m_apBlock[nLine][nLine2][vnPos.m_nX]->type == checkType)      //���u���b�N���I������悪���̏ꍇ
		{
			nLine--;
			nLine2++;
			while (nLine > vnPos.m_nZ&& nLine2 < vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(vnPos.m_nX, nLine2, nLine);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	int nLine3;

	//�E�㉜����
	listChange.clear();
	if (vnPos.m_nX< s_nNumBlockPerAxis - 1 && vnPos.m_nY< s_nNumBlockPerAxis - 1 && vnPos.m_nZ< s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nZ + 1;
		nLine3 = vnPos.m_nY + 1;
		while (m_apBlock[nLine2][nLine3][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 < s_nNumBlockPerAxis - 1
			&& nLine3 < s_nNumBlockPerAxis - 1)
		{
			nLine++;
			nLine2++;
			nLine3++;
		}
		if (m_apBlock[nLine2][nLine3][nLine]->type == checkType)
		{
			nLine--;
			nLine2--;
			nLine3--;
			while (nLine > vnPos.m_nX && nLine2 > vnPos.m_nZ && nLine3 > vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine3, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2--;
				nLine3--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//������O����
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nY >= 1 && vnPos.m_nZ >= 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nZ - 1;
		nLine3 = vnPos.m_nY - 1;
		while (m_apBlock[nLine2][nLine3][nLine]->type == blockType
			&& nLine >= 1
			&& nLine2 >= 1
			&& nLine3 >= 1)                                     //���u���b�N�������Ȃ���I���܂ł��
		{
			nLine--;
			nLine2--;
			nLine3--;
		}
		if (m_apBlock[nLine2][nLine3][nLine]->type == checkType)      //���u���b�N���I������悪���̏ꍇ
		{
			nLine++;
			nLine2++;
			nLine3++;
			while (nLine < vnPos.m_nX&& nLine3 < vnPos.m_nY&& nLine2 < vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine3, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2++;
				nLine3++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�E��������
	listChange.clear();
	if (vnPos.m_nX< s_nNumBlockPerAxis - 1 && vnPos.m_nY >= 1 && vnPos.m_nZ< s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nZ + 1;
		nLine3 = vnPos.m_nY - 1;
		while (m_apBlock[nLine2][nLine3][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 < s_nNumBlockPerAxis - 1
			&& nLine3 >= 1)             //���u���b�N�������Ȃ���I���܂ł��
		{
			nLine++;
			nLine2++;
			nLine3--;
		}
		if (m_apBlock[nLine2][nLine3][nLine]->type == checkType)      //���u���b�N���I������悪���̏ꍇ
		{
			nLine--;
			nLine2--;
			nLine3++;
			while (nLine > vnPos.m_nX&& nLine2 > vnPos.m_nZ&& nLine3 < vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine3, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2--;
				nLine3++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�����O����
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nY< s_nNumBlockPerAxis - 1 && vnPos.m_nZ >= 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nZ - 1;
		nLine3 = vnPos.m_nY + 1;
		while (m_apBlock[nLine2][nLine3][nLine]->type == blockType
			&& nLine >= 1
			&& nLine2 >= 1
			&& nLine3 < s_nNumBlockPerAxis - 1)
		{
			nLine--;
			nLine2--;
			nLine3++;
		}
		if (m_apBlock[nLine2][nLine3][nLine]->type == checkType)
		{
			nLine++;
			nLine2++;
			nLine3--;
			while (nLine < vnPos.m_nX&& nLine3 > vnPos.m_nY&& nLine2 < vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine3, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2++;
				nLine3--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�E����O����
	listChange.clear();
	if (vnPos.m_nX< s_nNumBlockPerAxis - 1 && vnPos.m_nY >= 1 && vnPos.m_nZ >= 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nZ - 1;
		nLine3 = vnPos.m_nY - 1;
		while (m_apBlock[nLine2][nLine3][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 >= 1
			&& nLine3 >= 1)
		{
			nLine++;
			nLine2--;
			nLine3--;
		}
		if (m_apBlock[nLine2][nLine3][nLine]->type == checkType)
		{
			nLine--;
			nLine2++;
			nLine3++;
			while (nLine > vnPos.m_nX&& nLine2 < vnPos.m_nZ&& nLine3 < vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine3, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2++;
				nLine3++;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//���㉜����
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nY< s_nNumBlockPerAxis - 1 && vnPos.m_nZ< s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nZ + 1;
		nLine3 = vnPos.m_nY + 1;
		while (m_apBlock[nLine2][nLine3][nLine]->type == blockType
			&& nLine >= 1
			&& nLine2 < s_nNumBlockPerAxis - 1
			&& nLine3 < s_nNumBlockPerAxis - 1)
		{
			nLine--;
			nLine2++;
			nLine3++;
		}
		if (m_apBlock[nLine2][nLine3][nLine]->type == checkType)
		{
			nLine++;
			nLine2--;
			nLine3--;
			while (nLine < vnPos.m_nX&& nLine3 > vnPos.m_nY&& nLine2 > vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine3, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2--;
				nLine3--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//�E���O����
	listChange.clear();
	if (vnPos.m_nX< s_nNumBlockPerAxis - 1 && vnPos.m_nY< s_nNumBlockPerAxis - 1 && vnPos.m_nZ >= 1)
	{
		nLine = vnPos.m_nX + 1;
		nLine2 = vnPos.m_nZ - 1;
		nLine3 = vnPos.m_nY + 1;
		while (m_apBlock[nLine2][nLine3][nLine]->type == blockType
			&& nLine < s_nNumBlockPerAxis - 1
			&& nLine2 >= 1
			&& nLine3 < s_nNumBlockPerAxis - 1)
		{
			nLine++;
			nLine2--;
			nLine3++;
		}
		if (m_apBlock[nLine2][nLine3][nLine]->type == checkType)
		{
			nLine--;
			nLine2++;
			nLine3--;
			while (nLine > vnPos.m_nX&& nLine2 < vnPos.m_nZ&& nLine3 > vnPos.m_nY)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine3, nLine2);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine--;
				nLine2++;
				nLine3--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	//����������
	listChange.clear();
	if (vnPos.m_nX >= 1 && vnPos.m_nY >= 1 && vnPos.m_nZ< s_nNumBlockPerAxis - 1)
	{
		nLine = vnPos.m_nX - 1;
		nLine2 = vnPos.m_nY - 1;
		nLine3 = vnPos.m_nZ + 1;
		//���u���b�N�������Ȃ���I���܂ł��
		while (m_apBlock[nLine3][nLine2][nLine]->type == blockType
			&& nLine >= 1 && nLine2 >= 1 && nLine3 < s_nNumBlockPerAxis - 1)
		{
			nLine--;
			nLine2--;
			nLine3++;
		}
		//���u���b�N���I������悪���̏ꍇ
		if (m_apBlock[nLine3][nLine2][nLine]->type == checkType)
		{
			nLine++;
			nLine2++;
			nLine3--;
			while (nLine < vnPos.m_nX&& nLine2 < vnPos.m_nY&& nLine3 > vnPos.m_nZ)
			{
				CKFVec3N vnPosBlock = CKFVec3N(nLine, nLine2, nLine3);
				listChange.push_back(vnPosBlock);
				bCanChoose = true;
				nLine++;
				nLine2++;
				nLine3--;
			}
		}
	}

	//�ύX����u���b�N�̃��X�g�Ńu���b�N��ݒ肷��
	SetByChangeList(listChange, checkType, pChangeBlock);

	return bCanChoose;
}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
CStage::BLOCK_TYPE CStage::GetBlockType(const int &nZ, const int &nY, const int &nX)
{
	if (nZ < 0 || nZ >= s_nNumBlockPerAxis
		|| nY < 0 || nY >= s_nNumBlockPerAxis
		|| nX < 0 || nX >= s_nNumBlockPerAxis)
	{
		return BT_NONE;
	}

	BLOCK_TYPE type = m_apBlock[nZ][nY][nX]->type;

	return type;
}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
CKFVec3 CStage::GetBlockRot(const int &nZ, const int &nY, const int &nX)
{
	if (nZ < 0 || nZ >= s_nNumBlockPerAxis
		|| nY < 0 || nY >= s_nNumBlockPerAxis
		|| nX < 0 || nX >= s_nNumBlockPerAxis)
	{
		return CKFVec3(0.0f);
	}

	CKFVec3 vRot = m_apBlock[nZ][nY][nX]->vRot;

	return vRot;
}