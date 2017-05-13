//--------------------------------------------------------------------------------
//
//　modelManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "manager.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CModel;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModelManager
{
public:
	enum MODEL_TYPE
	{
		XFILE,
		XFILE_MOTION,
	};

	enum DEMO_MODEL_NAME
	{
		DEMO_MODEL_CUBE,
		DEMO_MODEL_SELECTION_BOX,
		DEMO_MODEL_STAIRS,
		DEMO_MODEL_GOAL,
		DEMO_MODEL_MAX
	};

	CModelManager();
	~CModelManager() {}

	void	Init(void);
	void	LoadAll(const CManager::MODE &mode);
	void	UnloadAll(void);
	CModel	*GetModel(const int &nModelID);
private:
	struct MODEL_INFO
	{
		MODEL_TYPE	type;
		LPCSTR		path;
	};

	void Load(MODEL_INFO *pModelBegin, const int &nModelMax);

	std::vector<CModel*>	m_vectorModel;
	static MODEL_INFO		m_apDemoModelInfo[DEMO_MODEL_MAX];
};

#endif