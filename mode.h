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
class CGameObject;
class CCamera;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CMode
{
public:
	enum PRIORITY
	{
		PRI_3D,
		PRI_2D,
		PRI_MAX
	};

	CMode();
	~CMode() {}

	virtual HRESULT Init(void) = 0;
	virtual void	Uninit(void);
	virtual void	Update(void);
	virtual void	LateUpdate(void);
	virtual void	Draw(void);

	int				SaveGameObj(const PRIORITY &pri, CGameObject *pGameObj);
	void			ReleaseGameObj(const PRIORITY &pri, const int &nID);
	CCamera			*GetCamera(void) { return m_pCamera; }
protected:
	CCamera						*m_pCamera;
	std::vector<CGameObject*>	m_avectorGameObj[PRI_MAX];
};

#endif