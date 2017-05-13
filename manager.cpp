//--------------------------------------------------------------------------------
//
//�@manager.cpp
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "materialManager.h"
#include "rendererDX.h"
#include "inputDX.h"
#include "mode.h"
#include "modeDemo.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�[�ϐ��錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CManager::CManager()
	: m_pRenderer(NULL)
	, m_pKeyboard(NULL)
	, m_pMouse(NULL)
	, m_pTextureManager(NULL)
	, m_pLightManager(NULL)
	, m_pMaterialManager(NULL)
	, m_pModelManager(NULL)
	, m_mode(MODE_DEMO)
{
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		m_apMode[nCntMode] = NULL;
	}
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr;

	//�����_���[�̐���
	m_pRenderer = new CRendererDX;

	hr = m_pRenderer->Init(hWnd, bWindow);

	if (FAILED(hr))
	{
		MessageBox(NULL, "m_pRenderer->Init ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return hr;
	}

	//���͂̐���
	m_pKeyboard = new CKeyboardDX;
	m_pKeyboard->Init(hInstance, hWnd);
	m_pMouse = new CMouseDX;
	m_pMouse->Init(hInstance, hWnd);

	//���f���}�l�[�W���̐���
	m_pModelManager = new CModelManager;
	m_pModelManager->Init();

	//�e�N�X�`���}�l�[�W���̐���
	m_pTextureManager = new CTextureManager;
	m_pTextureManager->Init();

	//���C�g�}�l�[�W���̐���
	m_pLightManager = new CLightManager;
	m_pLightManager->Init();

	//�}�e���A���}�l�[�W���̐���
	m_pMaterialManager = new CMaterialManager;
	m_pMaterialManager->Init();

	//�������[�h�ݒ�
	m_mode = MODE_DEMO;

	//�e���[�h����
	m_apMode[MODE_DEMO] = new CModeDemo;

	//�������[�h�̏�����
	hr = m_apMode[m_mode]->Init();

	if (FAILED(hr))
	{
		MessageBox(NULL, "m_apMode[m_mode]::Init() ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return hr;
	}

	return hr;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	//���[�h�̔j��
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		if (m_apMode[nCntMode] != NULL)
		{
			m_apMode[nCntMode]->Uninit();
			delete m_apMode[nCntMode];
			m_apMode[nCntMode] = NULL;
		}
	}

	//�}�e���A���}�l�[�W���̔j��
	if (m_pMaterialManager != NULL)
	{
		delete m_pMaterialManager;
		m_pMaterialManager = NULL;
	}

	//���C�g�}�l�[�W���̔j��
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}

	//�e�N�X�`���}�l�[�W���̔j��
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->UnloadAll();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	//���f���}�l�[�W���̔j��
	if (m_pModelManager != NULL)
	{
		m_pModelManager->UnloadAll();
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	//���͂̔j��
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}

	//�����_���[�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CManager::Update(void)
{
	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pRenderer->Update();

	if (m_apMode[m_mode] != NULL)
	{
		m_apMode[m_mode]->Update();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CManager::LateUpdate(void)
{
	if (m_apMode[m_mode] != NULL)
	{
		m_apMode[m_mode]->LateUpdate();
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	m_pRenderer->Draw();
}