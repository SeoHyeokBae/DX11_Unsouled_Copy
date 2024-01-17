#include "pch.h"
#include "Inspector.h"

#include <Engine/CTransform.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"

Inspector::Inspector()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_arrComUI{}
{
	// 자식 UI 생성
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);
}

Inspector::~Inspector()
{
}

void Inspector::tick()
{
}

void Inspector::render_update()
{
	if (nullptr == m_TargetObject)
		return;

	string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	ImGui::Text(strName.c_str());
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			m_arrComUI[i]->SetTargetObject(_Object);
		}
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	m_TargetAsset = _Asset;
}

