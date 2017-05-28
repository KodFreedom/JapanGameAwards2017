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
#include "mode.h"
#include "modeTitle.h"
#include "gameObject2D.h"
#include "fade.h"
#include "inputDX.h"
#include "soundManager.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModeTitle::CModeTitle() : CMode()
{

}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CModeTitle::~CModeTitle()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CModeTitle::Init(void)
{
	CGameObject2D::Create(CKFVec3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0.0f, CKFVec2(SCREEN_WIDTH, SCREEN_HEIGHT), CTM::TEX_TITLE);
	GetManager()->GetSoundManager()->Play(CSM::BGM_TITLE);
	m_bStartLoop = false;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModeTitle::Uninit(void)
{
	GetManager()->GetSoundManager()->Stop(CSM::BGM_TITLE);
	GetManager()->GetSoundManager()->Stop(CSM::BGM_TITLE_LOOP);

	//�Q�[���I�u�W�F�N�g�̔j��
	CMode::Uninit();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModeTitle::Update(void)
{
	CMode::Update();

	CSoundManager *pSM = GetManager()->GetSoundManager();
	if (pSM->IsOver(CSM::BGM_TITLE) && !m_bStartLoop)
	{
		pSM->Play(CSM::BGM_TITLE_LOOP);
		m_bStartLoop = true;
	}

	CMouseDX* pMouse = GetManager()->GetMouse();

	if (pMouse->GetMouseTrigger(CMouseDX::MOUSE_LEFT))
	{
		pSM->Play(CSM::SE_CLICKSTART);
		GetManager()->GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE_DEMO);
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CModeTitle::LateUpdate(void)
{
	CMode::LateUpdate();
}