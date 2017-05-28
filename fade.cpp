//--------------------------------------------------------------------------------
//
//�@fade.cpp
//	Author : Xu Wenjie
//	Date   : 2016-11-15
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "fade.h"
#include "manager.h"
#include "textureManager.h"
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define FADE_RATE (1.0f / (120.0f * 0.5f))//�t�F�[�h�W��

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CFade::CFade()
{
	m_pVtxBuffer = NULL;
	m_fade = FADE_IN;
	m_modeNext = CManager::MODE_TITLE;
	m_cColor = CKFColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_nCnt = 0;
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CFade::~CFade()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CFade::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,				//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,					//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,						//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,					//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&m_pVtxBuffer,						//���_�o�b�t�@�̃A�h���X
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CFade : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ʒu�ݒ�
	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = CKFVec3(0.0f, 0.0f, 0.0f);
	pVtx[1].vPos = CKFVec3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].vPos = CKFVec3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].vPos = CKFVec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = m_cColor;
	pVtx[1].ulColor = m_cColor;
	pVtx[2].ulColor = m_cColor;
	pVtx[3].ulColor = m_cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = CKFVec2(0.0f, 0.0f);
	pVtx[1].vUV = CKFVec2(1.0f, 0.0f);
	pVtx[2].vUV = CKFVec2(0.0f, 1.0f);
	pVtx[3].vUV = CKFVec2(1.0f, 1.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//���z�A�h���X���
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CFade::Uninit(void)
{
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CFade::Update(void)
{
	if (m_fade == FADE_NONE) { return; }

	//fade���ԃJ�E���g
	if (m_nCnt != 0)
	{
		m_nCnt -= 1;
		return;
	}

	if (m_fade == FADE_IN)
	{
		m_cColor.m_fA -= FADE_RATE;//alpha�����Z���Č��̉�ʂ𕂂��オ�点��

		if (m_cColor.m_fA <= 0.0f)
		{
			m_cColor.m_fA = 0.0f;
			m_fade = FADE_NONE;
		}
	}
	else if (m_fade == FADE_OUT)
	{
		m_cColor.m_fA += FADE_RATE;

		if (m_cColor.m_fA >= 1.0f)
		{
			m_cColor.m_fA = 1.0f;
			m_fade = FADE_IN;
			GetManager()->SetMode(m_modeNext);
		}
	}

	SetColorFade(m_cColor);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,						//�X�g���[���ԍ�
		m_pVtxBuffer,			//���_�o�b�t�@
		0,						//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));		//�X�g���C�h��

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(CTM::TEX_FADE);
	pDevice->SetTexture(0, pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,						//�I�t�Z�b�g�i���_���j
		2);		//�|���S����
}

//--------------------------------------------------------------------------------
//  SetFade
//--------------------------------------------------------------------------------
void CFade::SetFade(const FADE &fade, const CManager::MODE &modeNext)
{
	if (m_fade == FADE_OUT) { return; }

	m_fade = fade;
	m_modeNext = modeNext;
	m_nCnt = 10;
}

//--------------------------------------------------------------------------------
//  SetColorFade
//--------------------------------------------------------------------------------
void CFade::SetColorFade(const CKFColor &cColor)
{
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���z�A�h���X���
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  �쐬
//--------------------------------------------------------------------------------
CFade *CFade::Create(void)
{
	CFade *pFade;
	pFade = new CFade;
	pFade->Init();
	return pFade;
}