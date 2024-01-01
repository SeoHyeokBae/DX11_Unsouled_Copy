#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTransform.h"

CRenderMgr::CRenderMgr()
	: m_pDebugObj(nullptr)
{

}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pDebugObj)
		delete m_pDebugObj;
}


void CRenderMgr::tick()
{
	// 윈도우 화면 클리어
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(ClearColor);

	render();

	render_debug();

	CDevice::GetInst()->Present();
}

void CRenderMgr::render()
{
	for (size_t i = 0; i < m_vecCam.size(); i++)
	{
		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_debug()
{
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Idx)
{
	if (m_vecCam.size() <= _Idx +1)
	{
		m_vecCam.resize(_Idx + 1);
	}
	
	// 동일한 우선순위의 카메라가 이미 존재하면 assert
	assert(nullptr == m_vecCam[_Idx]);

	m_vecCam[_Idx] = _Cam;
}