#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CStructuredBuffer.h"

#include "CTimeMgr.h"
#include "CDevice.h"
#include "CAssetMgr.h"
#include "components.h"

CRenderMgr::CRenderMgr()
	: m_Light2DBuffer(nullptr)
	, m_pDebugObj(nullptr)
	, m_DebugPosition(true)
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

	// Light2D update
	UpdateData();

	render();

	render_debug();

	// Light2D clear
	Clear();

	CDevice::GetInst()->Present();
}

void CRenderMgr::render()
{
	for (size_t i = 0; i < m_vecCam.size(); i++)
	{
		m_vecCam[i]->SortObject();
		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_debug()
{
	g_Transform.matView = m_vecCam[0]->GetViewMat();
	g_Transform.matProj = m_vecCam[0]->GetProjMat();

	list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	for (; iter != m_DbgShapeInfo.end();)
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CROSS:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CrossMesh"));
			break;
		case DEBUG_SHAPE::CUBE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
			break;
		default:
			break;
		}

		m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
		m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		// CROSS 용 TOPOLOGY 잠시 바꿔주기 위해 기존 TOPOLOGY 보관
		D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
		if (DEBUG_SHAPE::CROSS == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDebugObj->Transform()->UpdateData();

		m_pDebugObj->render();

		// CROSS TOPOLOGY 다시 되돌려줌
		m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);

		(*iter).fLifeTime += DT;
		if ((*iter).fDuration <= (*iter).fLifeTime)
		{
			iter = m_DbgShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::UpdateData()
{
	g_global.g_Light2DCount = (int)m_vecLight2D.size();
	//g_global.g_Light3DCount = (int)m_vecLight3D.size();

	// 전역 데이터 업데이트
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
	pCB->SetData(&g_global);
	pCB->UpdateData();

	// 2D 광원정보 업데이트
	static vector<tLightInfo> vecLight2DInfo;

	for (size_t i = 0; i < m_vecLight2D.size(); i++)
	{
		const tLightInfo& info = m_vecLight2D[i]->GetLightInfo();
		vecLight2DInfo.push_back(info);
	}

	// Light2D info 데이터를 구조화버퍼에 전달
	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	}
	m_Light2DBuffer->UpdateData(11);

	vecLight2DInfo.clear();
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
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