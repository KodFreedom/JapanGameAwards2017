//--------------------------------------------------------------------------------
//
//�@meshCube.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MESHCUBE_H_
#define _MESHCUBE_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CMeshCube : public CGameObject3D
{
public:
	CMeshCube();
	~CMeshCube() {}

	HRESULT	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vSize, const CKFColor &cColor);

	static CMeshCube *Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vSize, const CKFColor &cColor);
private:
	void MakeVertex(void);

	CKFVec3		m_vSize;
	CKFColor	m_cColor;
};

#endif