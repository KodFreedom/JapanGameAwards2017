//--------------------------------------------------------------------------------
//	Zpwb_[
//@KF_Math.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _KF_MATH_H_
#define _KF_MATH_H_

//--------------------------------------------------------------------------------
//  CN[ht@C
//--------------------------------------------------------------------------------
#include <math.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <d3dx9.h>

//--------------------------------------------------------------------------------
//  èè`
//--------------------------------------------------------------------------------

//~ü¦
#define KF_PI (3.1415926358979f)

//--------------------------------------------------------------------------------
//  ñ^è`
//--------------------------------------------------------------------------------

//¬÷©¸s©ÌÊð\·ñ^
enum KFRESULT
{
	KF_SUCCEEDED = 0,
	KF_FAILED = 1
};

//--------------------------------------------------------------------------------
//  NXè`
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ^è`
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Vector2
//--------------------------------------------------------------------------------
class CKFVec2
{
public:
	CKFVec2() : m_fX(0.0f), m_fY(0.0f) {}
	CKFVec2(const float &fX, const float &fY) : m_fX(fX), m_fY(fY) {}
	~CKFVec2() {}

	float m_fX;
	float m_fY;

	//LXg
	operator D3DXVECTOR2() const;

	//ZpZq
	CKFVec2 &operator=(const CKFVec2 &vValue);
	bool operator==(const CKFVec2 &vValue);

	CKFVec2 operator+(const CKFVec2 &vValue) const;
	void operator+=(const CKFVec2 &vValue);
};

//--------------------------------------------------------------------------------
//  Vector3
//--------------------------------------------------------------------------------
class CKFVec3
{
public:
	CKFVec3() : m_fX(0.0f), m_fY(0.0f), m_fZ(0.0f) {}
	CKFVec3(const float &fValue) : m_fX(fValue), m_fY(fValue), m_fZ(fValue) {}
	CKFVec3(const float &fX, const float &fY, const float &fZ) : m_fX(fX), m_fY(fY), m_fZ(fZ) {}
	~CKFVec3() {}

	float m_fX;
	float m_fY;
	float m_fZ;

	//LXg
	operator D3DXVECTOR3() const;

	//ZpZq
	CKFVec3 &operator=(const CKFVec3 &vValue);
	bool operator==(const CKFVec3 &vValue);
	bool operator!=(const CKFVec3 &vValue);

	CKFVec3 operator+(const CKFVec3 &vValue) const;
	void operator+=(const CKFVec3 &vValue);

	CKFVec3 operator-(const CKFVec3 &vValue) const;
	void operator-=(const CKFVec3 &vValue);

	CKFVec3 operator*(const float &fValue) const;
	void operator*=(const float &fValue);

	CKFVec3 operator*(const CKFVec3 &vValue) const;
	void operator*=(const CKFVec3 &vValue);

	CKFVec3 operator/(const float &fValue) const;
	void operator/=(const float &fValue);
};

//--------------------------------------------------------------------------------
//  Vector3(int)
//--------------------------------------------------------------------------------
class CKFVec3N
{
public:
	CKFVec3N() : m_nX(0), m_nY(0), m_nZ(0) {}
	CKFVec3N(const int &nX, const int &nY, const int &nZ) : m_nX(nX), m_nY(nY), m_nZ(nZ) {}
	~CKFVec3N() {}

	int m_nX;
	int m_nY;
	int m_nZ;
};

//--------------------------------------------------------------------------------
//  Matrix4*4
//--------------------------------------------------------------------------------
class CKFMtx44
{
public:
	CKFMtx44();
	CKFMtx44(
		const float &f11, const float &f12, const float &f13, const float &f14,
		const float &f21, const float &f22, const float &f23, const float &f24,
		const float &f31, const float &f32, const float &f33, const float &f34,
		const float &f41, const float &f42, const float &f43, const float &f44);
	~CKFMtx44() {}

	float m_af[4][4];

	//LXg
	operator D3DXMATRIX() const;

	//ZpZq
	CKFMtx44 &operator=(const CKFMtx44 &mtxValue);
	CKFMtx44 operator*(const CKFMtx44 &mtxValue) const;
	void operator*=(const CKFMtx44 &mtxValue);
};

//--------------------------------------------------------------------------------
//  Vector4
//--------------------------------------------------------------------------------
class CKFVec4
{
public:
	CKFVec4() : m_fX(0.0f), m_fY(0.0f), m_fZ(0.0f), m_fW(0.0f) {}
	CKFVec4(const float &fX, const float &fY, const float &fZ, const float &fW) : m_fX(fX), m_fY(fY), m_fZ(fZ), m_fW(fW) {}
	CKFVec4(const CKFVec3 &vVec, const float &fW) : m_fX(vVec.m_fX), m_fY(vVec.m_fY), m_fZ(vVec.m_fZ), m_fW(fW) {}
	~CKFVec4() {}

	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fW;

	//ZpZq
	CKFVec4 operator*(const CKFMtx44 &mtxValue) const;
	void operator*=(const CKFMtx44 &mtxValue);
};

//--------------------------------------------------------------------------------
//  Color
//--------------------------------------------------------------------------------
class CKFColor
{
public:
	CKFColor() : m_fR(0.0f), m_fG(0.0f), m_fB(0.0f), m_fA(0.0f) {}
	CKFColor(const float &fValue) : m_fR(fValue), m_fG(fValue), m_fB(fValue), m_fA(fValue) {}
	CKFColor(const float &fR, const float &fG, const float &fB, const float &fA) : m_fR(fR), m_fG(fG), m_fB(fB), m_fA(fA) {}
	~CKFColor() {}

	float m_fR;
	float m_fG;
	float m_fB;
	float m_fA;

	//LXg
	operator D3DCOLORVALUE () const;
	operator unsigned long() const;

	//ZpZq
	CKFColor &operator=(const CKFColor &vValue);
	bool operator==(const CKFColor &vValue);
};

//--------------------------------------------------------------------------------
//  Ray
//--------------------------------------------------------------------------------
class CKFRay
{
public:
	CKFRay() : m_vOrigin(CKFVec3(0.0f)), m_vDirection(CKFVec3(0.0f)) {}
	~CKFRay() {}

	CKFVec3 m_vOrigin;
	CKFVec3 m_vDirection;
};

//--------------------------------------------------------------------------------
//  vZ®
//--------------------------------------------------------------------------------
class CKFMath
{
public:
	//CÆXtBAÌ½èîñ
	struct RTS_INFO
	{
		bool bIsContact;
		float fTimingMin;
	};

	CKFMath() {}
	~CKFMath() {}

	//VectorvZ
	static float	VecMagnitude(const CKFVec2 &vValue);
	static float	VecMagnitude(const CKFVec3 &vValue);
	static void		VecNormalize(CKFVec2 *pVec);
	static void		VecNormalize(CKFVec3 *pVec);
	static float	Vec3Dot(const CKFVec3 &vVecL, const CKFVec3 &vVecR);
	static float	VecDistance(const CKFVec3 &vVecL, const CKFVec3 &vVecR);
	static void		Vec3TransformCoord(CKFVec3 *pVec, const CKFMtx44 &mtxRot);
	static void		Vec3TransformNormal(CKFVec3 *pVec, const CKFMtx44 &mtxRot);

	//MatrixvZ
	static CKFMtx44	ChangeDXMtxToMtx44(const D3DXMATRIX &mtx);
	static void		MtxIdentity(CKFMtx44 *pMtx);
	static void		MtxRotAxis(CKFMtx44 *pMtxRot, const CKFVec3 &vAxis, const float &fAngle);
	static void		MtxRotationYawPitchRoll(CKFMtx44 *pMtxRot, const CKFVec3 &vRot);
	static void		MtxTranslation(CKFMtx44 *pMtxTrans, const CKFVec3 &vPos);

	//RayvZ
	static CKFRay	CalculatePickingRay(const CKFVec2 &vScreenPos, const float &fViewportWidth, const float &fViewportHeight, const float &fProjMtx00, const float &fProjMtx11, const CKFMtx44 &mtxViewInverse);
	static CKFRay	ChangePosToRay(const CKFVec2 &vScreenPos, const float &fViewportWidth, const float &fViewportHeight, const float &fProjMtx00, const float &fProjMtx11);
	static void		TransformRay(CKFRay *pRay, const CKFMtx44 &mtxTrans);
	static RTS_INFO	ContactRayToSphere(const CKFRay &ray, const CKFVec3 &vSpherePos, const float &fRadius);

	//Ù©ÌvZ
	static void		NormalizeRotInTwoPi(float* pRot);
	static void		NormalizeRotInTwoPi(CKFVec3* pRot);
	static void		NormalizeRotInPi(float* pRot);
	static void		NormalizeRotInPi(CKFVec3* pRot);
	static float	CalculateZDepth(const CKFVec3 &vPos, const CKFVec3 &vCameraEye, const CKFVec3 &vCameraAt);
};

#endif