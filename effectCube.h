//--------------------------------------------------------------------------------
//	
//　effectCube.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _EFFECT_CUBE_H_
#define _EFFECT_CUBE_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObjectModel.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CEffectCube : public CGameObjectModel
{
public:
	enum CUBE_TYPE
	{
		CT_WHITE,
		CT_BLACK,
		CT_MAX
	};

	enum CUBE_DIR
	{
		CD_XP,	//X plus
		CD_XM,	//X minus
		CD_YP,	//Y plus
		CD_YM,	//Y minus
		CD_ZP,	//Z plus
		CD_ZM	//Z minus
	};

	CEffectCube();
	~CEffectCube() {}

	HRESULT Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CUBE_DIR &dir, const CUBE_TYPE &cubeType);
	void	Update(void);
	void	Draw(void);

	static CEffectCube* Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CUBE_DIR &dir, const CUBE_TYPE &cubeType);

private:
	static const float	s_fVelocityDefault;
	static const float	s_fRotSpeedDefault;
	static const float	s_fGravity;
	static const float	s_fDampling;
	static const float	s_fRotDampling;
	static const int	s_nLIfeMax = 120;

	int			m_nLife;
	CKFVec3		m_vVelocity;
	CKFVec3		m_vRotSpeed;
};

#endif