#include "pch.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CStructuredBuffer.h"

void CRenderMgr::init()
{
	// Structed Buffer ��
	Vec4 arr[3] =
	{
		Vec4(1.f,0.f,0.f,1.f),
		Vec4(0.f,1.f,0.f,1.f),
		Vec4(0.f,0.f,1.f,1.f)
	};

	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(Vec4), 2, SB_TYPE::READ_ONLY, true);

	// SetData���� Element�� ��� ������ ������� ó���� ���� �ܺο����� ���� ������� ���� �� ����
	if (m_Light2DBuffer->GetElementCount() < 3)
	{
		// ElementCount 3->10 �缳�� 
		m_Light2DBuffer->Create(sizeof(Vec4), 10, SB_TYPE::READ_ONLY, true);
	}

	m_Light2DBuffer->SetData(arr, 3);
	m_Light2DBuffer->UpdateData(14);

	// null�� arrTest�� �����͸� �о���� �͵� ����
	Vec4 arrTest[3] = {};
	m_Light2DBuffer->GetData(arrTest, 3);

	m_pDebugObj = new CGameObject;
	m_pDebugObj->AddComponent(new CTransform);
	m_pDebugObj->AddComponent(new CMeshRender);
}