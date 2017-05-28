//--------------------------------------------------------------------------------
//
//　player.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-27
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
#include "gameObject2D.h"
#include "gameObjectModel.h"
#include "heroin.h"
#include "stage.h"
#include "inputDX.h"
#include "soundManager.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CHeroin::CHeroin() : CGameObjectModel()
	, m_bClear(false)
	, m_fastForwardUI(NULL)
{
	m_param.bChanged = false;
	m_param.statusNext = HS_NORMAL;
	m_param.statusNow = HS_NORMAL;
	m_param.vForward = CKFVec3(0.0f);
	m_param.vForwardNext = CKFVec3(0.0f);
	m_param.vnNextPos = CKFVec3N(0, 0, 0);
	m_param.vnPos = CKFVec3N(0, 0, 0);
	m_param.vUp = CKFVec3(0.0f);
	m_param.vUpNext = CKFVec3(0.0f);
	m_listUndo.clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
KFRESULT CHeroin::Init(void)
{
	m_param.vUp = CKFVec3(0.0f, 1.0f, 0.0f);
	m_param.vForward = CKFVec3(0.0f, 0.0f, 1.0f);
	m_param.statusNext = HS_NORMAL;
	m_param.statusNow = HS_NORMAL;
	m_param.bChanged = false;

	int nHalf = 7 / 2;
	CKFVec3 vStart = CKFVec3(nHalf * -1.0f, nHalf * 1.0f, nHalf * -1.0f);

	for (int nCntZ = 0; nCntZ < 7; nCntZ++)
	{
		for (int nCntY = 0; nCntY < 7; nCntY++)
		{
			for (int nCntX = 0; nCntX < 7; nCntX++)
			{
				m_avPosOnStage[nCntZ][nCntY][nCntX] = vStart + CKFVec3(nCntX * 1.0f, nCntY * -1.0f, nCntZ * 1.0f);
			}
		}
	}

	CGameObjectModel::Init(m_avPosOnStage[m_param.vnPos.m_nZ][m_param.vnPos.m_nY][m_param.vnPos.m_nX], CKFVec3(0.0f, KF_PI * 0.5f, 0.0f), CModelManager::MODEL_HEROIN);

	m_fastForwardUI = CGameObject2D::Create(CKFVec3(SCREEN_WIDTH - 80.0f, SCREEN_HEIGHT * 0.5f, 0.0f), 0.0f, CKFVec2(160.0f, 40.0f), CTM::TEX_FAST_FORWARD);

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CHeroin::Update(void)
{
	CMouseDX *pMouse = GetManager()->GetMouse();
	POINT pos = pMouse->GetMousePos();
	int nLoop = 1;
	if (pMouse->GetMousePress(CMouseDX::MOUSE_LEFT)
		&& CheckFastForward(pos))
	{
		nLoop = 3;
	}

	for (int nCnt = 0; nCnt < nLoop; nCnt++)
	{
		CKFVec3 vNextPos = m_avPosOnStage[m_param.vnNextPos.m_nZ][m_param.vnNextPos.m_nY][m_param.vnNextPos.m_nX];
		CKFVec3 vPosNow = m_avPosOnStage[m_param.vnPos.m_nZ][m_param.vnPos.m_nY][m_param.vnPos.m_nX];
		CKFVec3 vPosMid = vPosNow + m_param.vForward * 0.5f;

		switch (m_param.statusNext)
		{
		case CHeroin::HS_UP:
			m_vPos += m_param.vForward * m_fSpeed;

			if (CKFMath::VecDistance(m_vPos, vPosMid) <= m_fSpeed)
			{
				m_vPos = vNextPos - m_param.vForward * 0.5f;
			}
			break;
		case CHeroin::HS_NORMAL:
			m_vPos += m_param.vForward * m_fSpeed;
			break;
		case CHeroin::HS_DOWN:
			m_vPos += m_param.vForward * m_fSpeed;

			if (CKFMath::VecDistance(m_vPos, vPosMid) <= m_fSpeed)
			{
				m_vPos = vNextPos - m_param.vForward * 0.5f;
			}
			break;
		case CHeroin::HS_STAIR_UP:

			if (m_param.bChanged == false)
			{//向き変換
				CKFVec3 vVecRight = m_param.vUp * m_param.vForward;
				CKFMath::VecNormalize(&vVecRight);
				m_param.vForward = vNextPos - vPosNow;
				CKFMath::VecNormalize(&m_param.vForward);

				m_param.vUp = m_param.vForward * vVecRight;
				CKFMath::VecNormalize(&m_param.vUp);
				m_param.bChanged = true;
			}

			m_vPos += m_param.vForward * m_fSpeed;
			break;
		case CHeroin::HS_STAIR_DOWN:

			if (m_param.bChanged == false)
			{//向き変換
				CKFVec3 vVecRight = m_param.vUp * m_param.vForward;
				CKFMath::VecNormalize(&vVecRight);
				m_param.vForward = vNextPos - vPosNow;
				CKFMath::VecNormalize(&m_param.vForward);

				m_param.vUp = m_param.vForward * vVecRight;
				CKFMath::VecNormalize(&m_param.vUp);
				m_param.bChanged = true;
			}

			m_vPos += m_param.vForward * m_fSpeed;
			break;
		case CHeroin::HS_NONE:
			return;
		default:
			break;
		}

		if (CKFMath::VecDistance(m_vPos, vNextPos) <= m_fSpeed)
		{//次の行先を探す
			if (m_param.statusNow == HS_STAIR_UP || m_param.statusNow == HS_STAIR_DOWN)
			{
				m_param.vForward = m_param.vForwardNext;
				m_param.vUp = m_param.vUpNext;
			}

			m_param.statusNow = m_param.statusNext;
			m_vPos = vNextPos;
			m_param.vnPos = m_param.vnNextPos;
			m_param.bChanged = false;

			CStage::BLOCK_TYPE type = GetManager()->GetModeNow()->GetStage()->GetBlockType(m_param.vnNextPos.m_nZ - 1, m_param.vnNextPos.m_nY - 1, m_param.vnNextPos.m_nX - 1);
			if (type == CStage::BT_GOAL)
			{
				m_bClear = true;
				return;
			}
			SearchNext();
		}
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CHeroin::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//  SearchNext
//--------------------------------------------------------------------------------
void CHeroin::SearchNext(void)
{
	CKFVec3N vnPosNext;
	CKFMtx44 mtxRot;

	if (m_param.statusNow != HS_STAIR_UP && m_param.statusNow != HS_STAIR_DOWN)
	{
		//前
		vnPosNext.m_nX = m_param.vnPos.m_nX + (int)m_param.vForward.m_fX;
		vnPosNext.m_nY = m_param.vnPos.m_nY - (int)m_param.vForward.m_fY;
		vnPosNext.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vForward.m_fZ;

		if (SearchHere(vnPosNext))
		{
			return;
		}

		//左
		CKFMath::MtxRotAxis(&mtxRot, m_param.vUp, -KF_PI * 0.5f);
		CKFMath::Vec3TransformNormal(&m_param.vForward, mtxRot);
		CKFMath::VecNormalize(&m_param.vForward);
		vnPosNext.m_nX = m_param.vnPos.m_nX + (int)m_param.vForward.m_fX;
		vnPosNext.m_nY = m_param.vnPos.m_nY - (int)m_param.vForward.m_fY;
		vnPosNext.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vForward.m_fZ;

		if (SearchHere(vnPosNext))
		{
			return;
		}

		//右
		CKFMath::MtxRotAxis(&mtxRot, m_param.vUp, KF_PI);
		CKFMath::Vec3TransformNormal(&m_param.vForward, mtxRot);
		CKFMath::VecNormalize(&m_param.vForward);
		vnPosNext.m_nX = m_param.vnPos.m_nX + (int)m_param.vForward.m_fX;
		vnPosNext.m_nY = m_param.vnPos.m_nY - (int)m_param.vForward.m_fY;
		vnPosNext.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vForward.m_fZ;

		if (SearchHere(vnPosNext))
		{
			return;
		}

		//後
		CKFMath::MtxRotAxis(&mtxRot, m_param.vUp, KF_PI * 0.5f);
		CKFMath::Vec3TransformNormal(&m_param.vForward, mtxRot);
		CKFMath::VecNormalize(&m_param.vForward);
		vnPosNext.m_nX = m_param.vnPos.m_nX + (int)m_param.vForward.m_fX;
		vnPosNext.m_nY = m_param.vnPos.m_nY - (int)m_param.vForward.m_fY;
		vnPosNext.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vForward.m_fZ;

		if (SearchHere(vnPosNext))
		{
			return;
		}
	}
	else
	{
		//前
		vnPosNext.m_nX = m_param.vnPos.m_nX + (int)m_param.vForwardNext.m_fX;
		vnPosNext.m_nY = m_param.vnPos.m_nY - (int)m_param.vForwardNext.m_fY;
		vnPosNext.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vForwardNext.m_fZ;

		if (SearchHere(vnPosNext))
		{
			return;
		}

		//後
		CKFMath::MtxRotAxis(&mtxRot, m_param.vUpNext, KF_PI);
		CKFMath::Vec3TransformNormal(&m_param.vForwardNext, mtxRot);
		CKFMath::VecNormalize(&m_param.vForwardNext);
		vnPosNext.m_nX = m_param.vnPos.m_nX + (int)m_param.vForwardNext.m_fX;
		vnPosNext.m_nY = m_param.vnPos.m_nY - (int)m_param.vForwardNext.m_fY;
		vnPosNext.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vForwardNext.m_fZ;

		if (SearchHere(vnPosNext))
		{
			return;
		}
	}
}

//--------------------------------------------------------------------------------
//  SearchHere
//--------------------------------------------------------------------------------
bool CHeroin::SearchHere(const CKFVec3N &pos)
{
	CKFVec3N vnPosNext;

	//上中下順番でチェック
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_param.statusNow != HS_STAIR_UP && m_param.statusNow != HS_STAIR_DOWN)
		{
			vnPosNext.m_nX = pos.m_nX - (nCnt - 1) * (int)m_param.vUp.m_fX;
			vnPosNext.m_nY = pos.m_nY + (nCnt - 1) * (int)m_param.vUp.m_fY;
			vnPosNext.m_nZ = pos.m_nZ - (nCnt - 1) * (int)m_param.vUp.m_fZ;
		}
		else
		{
			vnPosNext.m_nX = pos.m_nX - (nCnt - 1) * (int)m_param.vUpNext.m_fX;
			vnPosNext.m_nY = pos.m_nY + (nCnt - 1) * (int)m_param.vUpNext.m_fY;
			vnPosNext.m_nZ = pos.m_nZ - (nCnt - 1) * (int)m_param.vUpNext.m_fZ;
		}

		m_param.statusNext = (STATUS)nCnt;

		CStage::BLOCK_TYPE type;
		CKFVec3N vnPosUp;
		switch (m_param.statusNext)
		{
		case CHeroin::HS_UP:
			//チェックplayerの上
			if (m_param.statusNow != HS_STAIR_UP && m_param.statusNow != HS_STAIR_DOWN)
			{
				vnPosUp.m_nX = m_param.vnPos.m_nX + (int)m_param.vUp.m_fX;
				vnPosUp.m_nY = m_param.vnPos.m_nY - (int)m_param.vUp.m_fY;
				vnPosUp.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vUp.m_fZ;
			}
			else
			{
				vnPosUp.m_nX = m_param.vnPos.m_nX + (int)m_param.vUpNext.m_fX;
				vnPosUp.m_nY = m_param.vnPos.m_nY - (int)m_param.vUpNext.m_fY;
				vnPosUp.m_nZ = m_param.vnPos.m_nZ + (int)m_param.vUpNext.m_fZ;
			}

			type = GetManager()->GetModeNow()->GetStage()->GetBlockType(vnPosUp.m_nZ - 1, vnPosUp.m_nY - 1, vnPosUp.m_nX - 1);
			if (type != CStage::BT_NONE && type != CStage::BT_GOAL)
			{
				continue;
			}

			break;
		case CHeroin::HS_NORMAL:
			break;
		case CHeroin::HS_DOWN:
			//チェックブロックの上
			type = GetManager()->GetModeNow()->GetStage()->GetBlockType(pos.m_nZ - 1, pos.m_nY - 1, pos.m_nX - 1);
			if (type != CStage::BT_NONE && type != CStage::BT_GOAL)
			{
				continue;
			}

			break;
		default:
			break;
		}

		if (CanSheGomeHere(vnPosNext))
		{
			GomeHere(vnPosNext);
			return true;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------
//  CanSheGomeHere
//--------------------------------------------------------------------------------
bool CHeroin::CanSheGomeHere(const CKFVec3N &pos)
{
	//行先が範囲外にあるかをチェック
	if ((pos.m_nX < 0 || pos.m_nX > 6)
		|| (pos.m_nY < 0 || pos.m_nY > 6)
		|| (pos.m_nZ < 0 || pos.m_nZ > 6))
	{
		return false;
	}

	//行先がステージの範囲内にあるならブロックをチェック
	if ((pos.m_nX > 0 && pos.m_nX < 6)
		&& (pos.m_nY > 0 && pos.m_nY < 6)
		&& (pos.m_nZ > 0 && pos.m_nZ < 6))
	{
		CStage::BLOCK_TYPE blockType = GetManager()->GetModeNow()->GetStage()->GetBlockType(pos.m_nZ - 1, pos.m_nY - 1, pos.m_nX - 1);

		switch (blockType)
		{
		case CStage::BT_NONE:
			break;
		case CStage::BT_STAIRS:
			if (m_param.statusNext == HS_UP)
			{//上にあるならFalse
				return false;
			}
			else
			{//向きチェック
				CKFVec3 vRotStair = GetManager()->GetModeNow()->GetStage()->GetBlockRot(pos.m_nZ - 1, pos.m_nY - 1, pos.m_nX - 1);
				D3DXVECTOR3 vWorkForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				D3DXVECTOR3 vWorkUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				D3DXVECTOR3 vWorkForward2 = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				D3DXVECTOR3 vWorkUp2 = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				D3DXMATRIX mtxRot;
				D3DXMatrixRotationYawPitchRoll(&mtxRot, vRotStair.m_fY, vRotStair.m_fX, vRotStair.m_fZ);
				D3DXVec3TransformNormal(&vWorkForward, &vWorkForward, &mtxRot);
				D3DXVec3TransformNormal(&vWorkUp, &vWorkUp, &mtxRot);
				D3DXVec3TransformNormal(&vWorkForward2, &vWorkForward2, &mtxRot);
				D3DXVec3TransformNormal(&vWorkUp2, &vWorkUp2, &mtxRot);
				CKFVec3 vForwardStair, vUpStair, vForwardStair2, vUpStair2;
				vForwardStair.m_fX = vWorkForward.x;
				vForwardStair.m_fY = vWorkForward.y;
				vForwardStair.m_fZ = vWorkForward.z;
				CKFMath::VecNormalize(&vForwardStair);
				vUpStair.m_fX = vWorkUp.x;
				vUpStair.m_fY = vWorkUp.y;
				vUpStair.m_fZ = vWorkUp.z;
				CKFMath::VecNormalize(&vUpStair);
				vForwardStair2.m_fX = vWorkForward2.x;
				vForwardStair2.m_fY = vWorkForward2.y;
				vForwardStair2.m_fZ = vWorkForward2.z;
				CKFMath::VecNormalize(&vForwardStair2);
				vUpStair2.m_fX = vWorkUp2.x;
				vUpStair2.m_fY = vWorkUp2.y;
				vUpStair2.m_fZ = vWorkUp2.z;
				CKFMath::VecNormalize(&vUpStair2);

				//前なら上がり方向
				if (m_param.statusNext == HS_NORMAL)
				{
					if ((CKFMath::VecDistance(vForwardStair, m_param.vForward) <= 0.1f		//前方向一緒
						&& CKFMath::VecDistance(vUpStair, m_param.vUp) <= 0.1f)	//上方向一緒
						|| (CKFMath::VecDistance(vForwardStair2, m_param.vForward) <= 0.1f		//前方向一緒
							&& CKFMath::VecDistance(vUpStair2, m_param.vUp) <= 0.1f))	//上方向一緒
					{
						m_param.statusNext = HS_STAIR_UP;
						return true;
					}
				}

				//下なら下がり方向
				if (m_param.statusNext == HS_DOWN)
				{
					if ((CKFMath::VecDistance(vForwardStair, m_param.vForward * -1.0f) <= 0.1f		//前方向逆
						&& CKFMath::VecDistance(vUpStair, m_param.vUp) <= 0.1f)		//上方向一緒
						|| (CKFMath::VecDistance(vForwardStair2, m_param.vForward * -1.0f) <= 0.1f		//前方向逆
							&& CKFMath::VecDistance(vUpStair2, m_param.vUp) <= 0.1f))		//上方向一緒
					{
						m_param.statusNext = HS_STAIR_DOWN;
						return true;
					}
				}

				return false;
			}
			break;
		case CStage::BT_GOAL:
			break;
		default:
			return false;
		}
	}

	//チェック地面がステージの範囲内にあるかどうか
	CKFVec3N posUnder;
	if (m_param.statusNow != HS_STAIR_UP && m_param.statusNow != HS_STAIR_DOWN)
	{
		posUnder.m_nX = pos.m_nX - (int)m_param.vUp.m_fX;
		posUnder.m_nY = pos.m_nY + (int)m_param.vUp.m_fY;
		posUnder.m_nZ = pos.m_nZ - (int)m_param.vUp.m_fZ;
	}
	else
	{
		posUnder.m_nX = pos.m_nX - (int)m_param.vUpNext.m_fX;
		posUnder.m_nY = pos.m_nY + (int)m_param.vUpNext.m_fY;
		posUnder.m_nZ = pos.m_nZ - (int)m_param.vUpNext.m_fZ;
	}

	if ((posUnder.m_nX <= 0 || posUnder.m_nX >= 6)
		|| (posUnder.m_nY <= 0 || posUnder.m_nY >= 6)
		|| (posUnder.m_nZ <= 0 || posUnder.m_nZ >= 6))
	{//範囲外ならいけない
		return false;
	}
	else
	{//地面のタイプをチェック
		CStage::BLOCK_TYPE blockType = GetManager()->GetModeNow()->GetStage()->GetBlockType(posUnder.m_nZ - 1, posUnder.m_nY - 1, posUnder.m_nX - 1);

		if (blockType == CStage::BT_WHITE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CHeroin::GomeHere(const CKFVec3N &pos)
{
	m_param.vnNextPos = pos;

	if (m_param.statusNext == HS_STAIR_UP || m_param.statusNext == HS_STAIR_DOWN)
	{
		m_param.vForwardNext = m_param.vUp;
		m_param.vUpNext = m_param.vForward;
	}

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CHeroin::SetHeroin(const CKFVec3N &vnPos)
{
	m_param.vnPos = vnPos;
	m_param.vnNextPos = m_param.vnPos;
	m_param.vUp = CKFVec3(0.0f, 1.0f, 0.0f);
	m_param.vForward = CKFVec3(0.0f, 0.0f, 1.0f);
	m_param.statusNext = HS_NORMAL;
	m_param.statusNow = HS_NORMAL;
	m_param.bChanged = false;
	m_bClear = false;
	CGameObjectModel::Init(m_avPosOnStage[m_param.vnPos.m_nZ][m_param.vnPos.m_nY][m_param.vnPos.m_nX], CKFVec3(0.0f, KF_PI * 0.5f, 0.0f), CModelManager::MODEL_HEROIN);
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CHeroin *CHeroin::Create(void)
{
	CHeroin *pPlayer = NULL;
	pPlayer = new CHeroin;
	pPlayer->Init();
	pPlayer->m_pri = GOM::PRI_3D;
	pPlayer->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pPlayer);
	return pPlayer;
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
bool CHeroin::CheckFastForward(POINT pos)
{
	CKFVec3 vPosFastForward = CKFVec3(SCREEN_WIDTH - 80.0f, SCREEN_HEIGHT * 0.5f, 0.0f);
	CKFVec3 vPosMouse = CKFVec3((float)pos.x, (float)pos.y, 0.0f);
	if (vPosMouse.m_fX >= vPosFastForward.m_fX - 80.0f && vPosMouse.m_fX <= vPosFastForward.m_fX + 80.0f
		&& vPosMouse.m_fY >= vPosFastForward.m_fY - 20.0f && vPosMouse.m_fY <= vPosFastForward.m_fY + 20.0f)
	{
		return true;
	}
	return false;
}