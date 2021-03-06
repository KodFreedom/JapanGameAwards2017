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
#define CMOM CModelManager //CModelManagerの略称

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

	enum MODEL_NAME
	{
		MODEL_NONE,
		MODEL_CUBE,
		MODEL_SELECTION_BOX,
		MODEL_STAIRS,
		MODEL_GOAL,
		MODEL_WIREBOX,
		MODEL_HEROIN,
		MODEL_MAX
	};

	CModelManager();
	~CModelManager() {}

	void	Init(void);
	void	LoadAll(void);
	void	UnloadAll(void);
	CModel	*GetModel(const MODEL_NAME &modelName);
private:
	struct MODEL_INFO
	{
		MODEL_TYPE	type;
		LPCSTR		path;
	};

	void Load(const MODEL_NAME &modelBegin, const MODEL_NAME &modelEnd);

	std::vector<CModel*>	m_vectorModel;
	static MODEL_INFO		m_apModelInfo[MODEL_MAX];
};

#endif