#include "pch.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CStructuredBuffer.h"

void CRenderMgr::init()
{
	// Structed Buffer 예
	Vec4 arr[3] =
	{
		Vec4(1.f,0.f,0.f,1.f),
		Vec4(0.f,1.f,0.f,1.f),
		Vec4(0.f,0.f,1.f,1.f)
	};

	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(Vec4), 2, SB_TYPE::READ_ONLY, true);

	// SetData에서 Element의 요소 갯수가 많을경우 처리와 같이 외부에서도 다음 방법으로 해줄 수 있음
	if (m_Light2DBuffer->GetElementCount() < 3)
	{
		// ElementCount 3->10 재설정 
		m_Light2DBuffer->Create(sizeof(Vec4), 10, SB_TYPE::READ_ONLY, true);
	}

	m_Light2DBuffer->SetData(arr, 3);
	m_Light2DBuffer->UpdateData(14);

	// null인 arrTest에 데이터를 읽어오는 것도 가능
	Vec4 arrTest[3] = {};
	m_Light2DBuffer->GetData(arrTest, 3);

	m_pDebugObj = new CGameObject;
	m_pDebugObj->AddComponent(new CTransform);
	m_pDebugObj->AddComponent(new CMeshRender);
}