//--------------------------------------------------------------------------------
//
//　soundManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define CSM CSoundManager	//マネージャの略称

//--------------------------------------------------------------------------------
//  クラス宣言
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
	//パラメータ構造体定義
	typedef struct
	{
		std::string strFileName;	// ファイル名
		int nCntLoop;				// ループカウント
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2*				m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice*	m_pMasteringVoice;					// マスターボイス
	IXAudio2SourceVoice*	m_apSourceVoice[SL_MAX];	// ソースボイス
	BYTE*					m_apDataAudio[SL_MAX];		// オーディオデータ
	DWORD					m_aSizeAudio[SL_MAX];		// オーディオデータサイズ

	static SOUNDPARAM 		m_aParam[SL_MAX];
};

#endif