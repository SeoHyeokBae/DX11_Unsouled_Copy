#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_FOV(XM_PI / 2.f)
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
{
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = vResol.x / vResol.y;
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
	// 뷰 행렬을 계산한다
	Vec3 vCamPos = Transform()->GetRelativePos();

	m_matView = XMMatrixIdentity();

	
	m_matView._41 = -vCamPos.x;
	m_matView._42 = -vCamPos.y;
	m_matView._43 = -vCamPos.z;
	
	m_matProj = XMMatrixIdentity();

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교 투영
		Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
		m_matProj = XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far); // 종횡비,거리
	}
	else
	{
		// 원근 투영
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
	
	// 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}