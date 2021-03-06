//--------------------------------------------------------------------------------
//	DX用便利関数
//　KF_UtilityDX.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _KF_UTILITY_DX_H_
#define _KF_UTILITY_DX_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "KF_Math.h"

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class CKFUtilityDX
{
public:
	CKFUtilityDX() {}
	~CKFUtilityDX() {}

	static CKFRay	ChangeToPickingRay(const CKFVec2 &vScreenPos);
};

#endif