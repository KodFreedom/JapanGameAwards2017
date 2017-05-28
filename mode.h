//--------------------------------------------------------------------------------
//
//　mode.h
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODE_H_
#define _MODE_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CCamera;
class CStage;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CMode
{
public:
	CMode();
	~CMode() {}

	virtual void	Init(void) = 0;
	virtual void	Uninit(void);
	virtual void	Update(void);
	virtual void	LateUpdate(void);
	virtual void	Draw(void);

	//Get関数
	CCamera*		GetCamera(void) { return m_pCamera; }
	CStage*			GetStage(void) { return m_pStage; }

protected:
	CCamera*		m_pCamera;
	CStage*			m_pStage;
	bool			m_bStartLoop;
};

#endif