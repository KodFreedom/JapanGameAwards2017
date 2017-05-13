//--------------------------------------------------------------------------------
//
//�@modelX.cpp
//	Author : Xu Wenjie
//	Date   : 2017-01-23
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
#include "model.h"
#include "modelX.h"

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModelX::CModelX() : CModel()
	, m_pMesh(NULL)
	, m_pBufferMaterial(NULL)
	, m_dwNumMaterial(0)
{
	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
HRESULT CModelX::Init(const LPCSTR &pXFilePath)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//�n�[�h�f�B�X�N����X�t�@�C���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(
		pXFilePath,				//�t�@�C����
		D3DXMESH_MANAGED,
		pDevice,
		NULL,					//�אڏ��
		&m_pBufferMaterial,		//���f���̃}�e���A�����
		NULL,
		&m_dwNumMaterial,		//���f���̃}�e���A����
		&m_pMesh);				//���b�V�����

	if (FAILED(hr))
	{
		MessageBox(NULL, "CModelX : D3DXLoadMeshFromX ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return hr;
	}

	//texture
	D3DXMATERIAL *pMat;//�}�e���A�������󂯎���p�̃|�C���^
	
	pMat = (D3DXMATERIAL*)m_pBufferMaterial->GetBufferPointer();
	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			LPDIRECT3DTEXTURE9 pTexture = NULL;

			//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
			//���G���[�`�F�b�N�K�{
			hr = D3DXCreateTextureFromFile(pDevice,
				pMat[nCnt].pTextureFilename,
				&pTexture);

			if (FAILED(hr))
			{
				char aStr[128];
				wsprintf(aStr, "CModelX %d�Ԃ̃e�N�X�`�� ERROR!", nCnt);
				MessageBox(NULL, aStr, "�G���[", MB_OK | MB_ICONWARNING);
			}

			m_vectorTexture.push_back(pTexture);
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModelX::Uninit(void)
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (m_pBufferMaterial != NULL)
	{
		m_pBufferMaterial->Release();
		m_pBufferMaterial = NULL;
	}

	for (int nCnt = 0; nCnt < (int)m_vectorTexture.size(); nCnt++)
	{
		if (m_vectorTexture[nCnt] != NULL)
		{
			m_vectorTexture[nCnt]->Release();
			m_vectorTexture[nCnt] = NULL;
		}
	}

	m_vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CModelX::Draw(LPDIRECT3DDEVICE9 pDevice)
{
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);//���݃f�o�C�X�ɐݒ肳��Ă�}�e���A�������擾
	D3DXMATERIAL *pMat;//�}�e���A�������󂯎���p�̃|�C���^
	pMat = (D3DXMATERIAL*)m_pBufferMaterial->GetBufferPointer();

	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCnt].MatD3D);
		
		//Texture
		pDevice->SetTexture(0, m_vectorTexture[nCnt]);

		//���b�V���̕`��
		m_pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  �`�揈��(�}�e���A���w��)
//--------------------------------------------------------------------------------
void CModelX::Draw(LPDIRECT3DDEVICE9 pDevice, const CMM::MATERIAL &matType)
{
	//���݃f�o�C�X�ɐݒ肳��Ă�}�e���A�������擾
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		//Texture
		pDevice->SetTexture(0, m_vectorTexture[nCnt]);

		//���b�V���̕`��
		m_pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  �`�揈��(�}�e���A���w��, �e�N�X�`���w��)
//--------------------------------------------------------------------------------
void CModelX::Draw(LPDIRECT3DDEVICE9 pDevice, const CMM::MATERIAL &matType, const int &nTexID)
{
	//���݃f�o�C�X�ɐݒ肳��Ă�}�e���A�������擾
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(nTexID);
	pDevice->SetTexture(0, pTexture);

	for (int nCnt = 0; nCnt < (int)m_dwNumMaterial; nCnt++)
	{
		//���b�V���̕`��
		m_pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  �쐬
//--------------------------------------------------------------------------------
CModelX *CModelX::Create(const LPCSTR pXFilePath)
{
	CModelX *pModelX = NULL;
	pModelX = new CModelX;
	pModelX->Init(pXFilePath);
	return pModelX;
}
