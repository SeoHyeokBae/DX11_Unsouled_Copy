#include "pch.h"
#include "Inspector.h"


Inspector::Inspector()
	: UI("Inspector","##Inspector")
{
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
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	m_TargetAsset = _Asset;
}

