#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	// 월드 위치값을 LightInfo 멤버에 갱신해 놓는다.
	Vec3 vWorldPos = Transform()->GetWorldPos();
	m_Info.vWorldPos = vWorldPos;

	m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	//float aa = acos(cos((160.f) * (3.141592 / 180.f)));

	// 광원 등록
	CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SetLightType(LIGHT_TYPE _type)
{
	m_Info.LightType = (int)_type;
}

void CLight2D::SetRadius(float _Radius)
{
	m_Info.fRadius = _Radius;
}

void CLight2D::SetAngle(float _Angle)
{
	m_Info.fAngle = _Angle;
}