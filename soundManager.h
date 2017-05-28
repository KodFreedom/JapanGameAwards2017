//--------------------------------------------------------------------------------
//
//�@soundManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CSM CSoundManager	//�}�l�[�W���̗���

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CSoundManager
{
public:
	typedef enum
	{
		BGM_TITLE = 0,
		BGM_TITLE_LOOP,
		BGM_GAME,
		BGM_GAME_LOOP,
		SE_CLICKSTART,
		SE_COLORCHANGE,
		SE_UICLICK,
		SL_MAX,
	} SOUND_LABEL;

	CSoundManager();
	~CSoundManager() {}

	HRESULT	LoadAll(void);
	void	UnloadAll(void);
	void	Play(SOUND_LABEL label);
	void	Stop(SOUND_LABEL label);
	bool	IsOver(SOUND_LABEL label);
	bool	IsPlaying(SOUND_LABEL label);
	void	StopAll(void);

private:
	//�p�����[�^�\���̒�`
	typedef struct
	{
		std::string strFileName;	// �t�@�C����
		int nCntLoop;				// ���[�v�J�E���g
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2*				m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice*	m_pMasteringVoice;					// �}�X�^�[�{�C�X
	IXAudio2SourceVoice*	m_apSourceVoice[SL_MAX];	// �\�[�X�{�C�X
	BYTE*					m_apDataAudio[SL_MAX];		// �I�[�f�B�I�f�[�^
	DWORD					m_aSizeAudio[SL_MAX];		// �I�[�f�B�I�f�[�^�T�C�Y

	static SOUNDPARAM 		m_aParam[SL_MAX];
};

#endif