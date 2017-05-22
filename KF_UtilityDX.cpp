//--------------------------------------------------------------------------------
//	DX用便利関数
//　KF_UtilityDX.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "KF_UtilityDX.h"
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "mode.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ChangeToPickingRay
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
CKFRay CKFUtilityDX::ChangeToPickingRay(const CKFVec2 &vScreenPos)
{
	CKFVec2 vPos3D;
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	D3DVIEWPORT9 vp;
	pDevice->GetViewport(&vp);

	D3DXMATRIX mtxProjection;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

	CCamera *pCamera = GetManager()->GetModeNow()->GetCamera();
	D3DXMATRIX mtx = pCamera->GetMtxViewInverse();
	CKFMtx44 mtx44 = CKFMath::ChangeDXMtxToMtx44(mtx);

	CKFRay ray = CKFMath::CalculatePickingRay(vScreenPos, vp.Width, vp.Height, mtxProjection(0, 0), mtxProjection(1, 1), mtx44);

	return ray;
}