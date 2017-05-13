//--------------------------------------------------------------------------------
//
//�@modeDemo.cpp
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
#include "manager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "mode.h"
#include "modeDemo.h"
#include "gameObject.h"
#include "gameObject3D.h"
#include "gameObjectModel.h"
#include "meshField.h"
#include "meshCube.h"
#include "stage.h"
#include "stageEditor.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModeDemo::CModeDemo() : CMode()
{

}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CModeDemo::~CModeDemo()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
HRESULT CModeDemo::Init(void)
{
	//�e�N�X�`���̓ǂݍ���
	GetManager()->GetTextureManager()->LoadAll(CManager::MODE_DEMO);
	
	//���f���̓ǂݍ���
	GetManager()->GetModelManager()->LoadAll(CManager::MODE_DEMO);

	//�Q�[���I�u�W�F�N�g�̏�����
	//CMeshField::Create(1, 1, CKFVec2(1.0f, 1.0f), CKFVec3(0.0f), CKFVec3(0.0f));
	//CMeshCube::Create(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f), CKFColor(1.0f));
	//CGameObjectModel::Create(CKFVec3(0.0f), CKFVec3(0.0f), CModelManager::DEMO_MODEL_CUBE);
	CStageEditor::Create(CKFVec3(0.0f), CKFVec3(0.0f));

	//���C�g�̏�����
	GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//�J�����̏�����
	m_pCamera = new CCamera;
	m_pCamera->Init();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModeDemo::Uninit(void)
{
	//�Q�[���I�u�W�F�N�g�̔j��
	CMode::Uninit();

	//���C�g�̔j��
	GetManager()->GetLightManager()->ReleaseAll();

	//�J�����̔j��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//���f���̔j��
	GetManager()->GetModelManager()->UnloadAll();

	//�e�N�X�`���̔j��
	GetManager()->GetTextureManager()->UnloadAll();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();

	m_pCamera->Update();
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();

	m_pCamera->LateUpdate();
}