//--------------------------------------------------------------------------------
//
//�@gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "mode.h"
#include "modelManager.h"
#include "model.h"
#include "gameObject.h"
#include "gameObjectModel.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectModel::CGameObjectModel() : CGameObject()
	, m_vRot(CKFVec3(0.0f))
	, m_nModelID(-1)
	, m_matType(CMM::MAT_WHITE)
{

}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectModel::~CGameObjectModel()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
HRESULT CGameObjectModel::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nModelID)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_nModelID = nModelID;

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CGameObjectModel::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectModel::Update(void)
{

}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CGameObjectModel::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CGameObjectModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	CModel *pModel = GetManager()->GetModelManager()->GetModel(m_nModelID);

	if (pModel != NULL)
	{
		//RenderState�ݒ�
		SetRenderState(pDevice);

		//�}�g���b�N�X�ݒ�
		SetMatrix(pDevice);

		//�`��
		if (m_matType != CMM::MAT_MAX)
		{
			pModel->Draw(pDevice, m_matType);
		}
		else
		{
			pModel->Draw(pDevice);
		}

		//RenderState�߂�
		ResetRenderState(pDevice);
	}
}

//--------------------------------------------------------------------------------
// SetWorldMatrix
//--------------------------------------------------------------------------------
void CGameObjectModel::SetMatrix(LPDIRECT3DDEVICE9 pDevice)
{
	//���[���h���΍��W
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&mtxWorld);

	//��](Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);
	mtxWorld *= mtxRot;

	//���s�ړ�
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);
	mtxWorld *= mtxPos;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//--------------------------------------------------------------------------------
// SetRenderState
//--------------------------------------------------------------------------------
void CGameObjectModel::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{

}

//--------------------------------------------------------------------------------
// ResetRenderState
//--------------------------------------------------------------------------------
void CGameObjectModel::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{

}

//--------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------
CGameObjectModel *CGameObjectModel::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const int &nModelID)
{
	CGameObjectModel *pObjModel = NULL;
	pObjModel = new CGameObjectModel;
	pObjModel->Init(vPos, vRot, nModelID);
	pObjModel->m_pri = CMode::PRI_3D;
	pObjModel->m_nID = GetManager()->GetModeNow()->SaveGameObj(CMode::PRI_3D, pObjModel);
	return pObjModel;
}