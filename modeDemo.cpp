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
#include "fade.h"
#include "inputDX.h"
#include "soundManager.h"

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
void CModeDemo::Init(void)
{
	//�Q�[���I�u�W�F�N�g�̏�����
	m_pStage = CStage::Create(CKFVec3(0.0f), CKFVec3(0.0f), 0);
	//CStageEditor::Create(CKFVec3(0.0f), CKFVec3(0.0f));

	//���C�g�̏�����
	GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//�J�����̏�����
	m_pCamera = new CCamera;
	m_pCamera->Init();

	GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
	m_bStartLoop = false;
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

	GetManager()->GetSoundManager()->Stop(CSM::BGM_GAME);
	GetManager()->GetSoundManager()->Stop(CSM::BGM_GAME_LOOP);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();

	m_pCamera->Update();

	CSoundManager *pSM = GetManager()->GetSoundManager();
	if (pSM->IsOver(CSM::BGM_GAME) && !m_bStartLoop)
	{
		pSM->Play(CSM::BGM_GAME_LOOP);
		m_bStartLoop = true;
	}

	CKeyboardDX *pKeyboard = GetManager()->GetKeyboard();
	if (pKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		GetManager()->GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();

	m_pCamera->LateUpdate();
}