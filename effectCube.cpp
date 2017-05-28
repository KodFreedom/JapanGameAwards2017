//--------------------------------------------------------------------------------
//	
//　effectCube.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "mode.h"
#include "modelManager.h"
#include "model.h"
#include "gameObject.h"
#include "effectCube.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float CEffectCube::s_fVelocityDefault = 0.1f;
const float CEffectCube::s_fRotSpeedDefault = 0.01f;
const float CEffectCube::s_fGravity = 9.81f * 0.05f / TIMER_INTERVAL;
const float CEffectCube::s_fDampling = 0.90f;
const float CEffectCube::s_fRotDampling = 0.98f;

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CEffectCube::CEffectCube() : CGameObjectModel()
	, m_vVelocity(CKFVec3(0.0f))
	, m_vRotSpeed(CKFVec3(0.0f))
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
HRESULT CEffectCube::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CUBE_DIR &dir, const CUBE_TYPE &cubeType)
{
	CGameObjectModel::Init(vPos, vRot, CModelManager::MODEL_CUBE);

	switch (dir)
	{
	case CD_XP:
		m_vVelocity = CKFVec3(1.0f, 0.0f, 0.0f) * s_fVelocityDefault;
		m_vRotSpeed = CKFVec3(0.0f, 0.0f, 1.0f) * s_fRotSpeedDefault;
		break;
	case CD_XM:
		m_vVelocity = CKFVec3(-1.0f, 0.0f, 0.0f) * s_fVelocityDefault;
		m_vRotSpeed = CKFVec3(0.0f, 0.0f, -1.0f) * s_fRotSpeedDefault;
		break;
	case CD_YP:
		m_vVelocity = CKFVec3(0.0f);
		m_vRotSpeed = CKFVec3(0.0f);
		break;
	case CD_YM:
		m_vVelocity = CKFVec3(0.0f);
		m_vRotSpeed = CKFVec3(0.0f);
		break;
	case CD_ZP:
		m_vVelocity = CKFVec3(0.0f, 0.0f, 1.0f) * s_fVelocityDefault;
		m_vRotSpeed = CKFVec3(1.0f, 0.0f, 0.0f) * s_fRotSpeedDefault;
		break;
	case CD_ZM:
		m_vVelocity = CKFVec3(0.0f, 0.0f, -1.0f) * s_fVelocityDefault;
		m_vRotSpeed = CKFVec3(-1.0f, 0.0f, 0.0f) * s_fRotSpeedDefault;
		break;
	default:
		break;
	}

	switch (cubeType)
	{
	case CT_WHITE:
		m_matType = CMM::MAT_WHITE;
		break;
	case CT_BLACK:
		m_matType = CMM::MAT_BLACK;
		break;
	default:
		break;
	}

	m_nLife = s_nLIfeMax;

	return S_OK;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CEffectCube::Update(void)
{
	m_vPos += m_vVelocity;
	m_vRot += m_vRotSpeed;

	m_vVelocity.m_fY -= s_fGravity;
	m_vVelocity *= s_fDampling;
	m_vRotSpeed *= s_fRotDampling;

	m_nLife--;
	if (m_nLife <= 0)
	{
		CGameObject::Release();
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CEffectCube::Draw(void)
{
	CGameObjectModel::Draw();
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CEffectCube* CEffectCube::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CUBE_DIR &dir, const CUBE_TYPE &cubeType)
{
	CEffectCube *pCube = NULL;
	pCube = new CEffectCube;
	pCube->Init(vPos, vRot, dir, cubeType);
	pCube->m_pri = GOM::PRI_3D;
	pCube->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pCube);
	return pCube;
}