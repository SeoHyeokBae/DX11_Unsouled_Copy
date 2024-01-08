#include "pch.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CStructuredBuffer.h"

void CRenderMgr::init()
{
	// Structed Buffer ¿¹
	Vec4 arr[3] =
	{
		Vec4(1.f,0.f,0.f,1.f),
		Vec4(0.f,1.f,0.f,1.f),
		Vec4(0.f,0.f,1.f,1.f)
	};

	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(Vec4), 3, SB_TYPE::READ_ONLY, arr);
	m_Light2DBuffer->UpdateData(14);

	m_pDebugObj = new CGameObject;
	m_pDebugObj->AddComponent(new CTransform);
	m_pDebugObj->AddComponent(new CMeshRender);
}