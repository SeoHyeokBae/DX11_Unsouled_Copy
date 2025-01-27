#include "pch.h"
#include "ComponentUI.h"


ComponentUI::ComponentUI(const string& _strName, const string& _ID, COMPONENT_TYPE _Type)
	: UI(_strName, _ID)
	, m_TargetObject(nullptr)
	, m_Type(_Type)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::render_update()
{
	// ComponentUI 공용 머리말
	ImGui::Separator();	// 분리 줄
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));			// 버튼색
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));	// 버튼 마우스 위
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));	// 버튼 눌렀을 시 
																							// 모두 같은색으로 색효과만
	ImGui::Button(m_ComponentTitle.c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void ComponentUI::SetTargetObject(CGameObject* _Target)
{
	m_TargetObject = _Target;

	// 타겟 오브젝트가 없거나, 타겟 오브젝트가 컴포넌트UI 담당 컴포넌트를 보유하고 있지 않다며
	// 컴포넌트UI 를 비활성화 한다.
	if (nullptr == m_TargetObject)
	{
		Deactivate();
	}

	else
	{
		if (!m_TargetObject->GetComponent(m_Type))	// 컴포넌트가 일치하지 않다면 
		{
			Deactivate();
		}
		else
		{
			Activate();
		}
	}

}


