//--------------------------------------------------------------------------------
//
//　modeDemo.h
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEDEMO_H_
#define _MODEDEMO_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModeDemo : public CMode
{
public:
	CModeDemo();
	~CModeDemo();

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);

private:
	
};

#endif