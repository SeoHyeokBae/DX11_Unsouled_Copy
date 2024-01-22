#include "pch.h"
#include "CameraUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>

CameraUI::CameraUI()
	: ComponentUI("Camera","##Camera", COMPONENT_TYPE::CAMERA)
{
	SetSize(ImVec2(0.f, 150.f));
	SetComponentTitle("Camera");

}

CameraUI::~CameraUI()
{
}

void CameraUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	// 투영 선택
	const char* Projitems[] = { "ORTHOGRAPHIC" , "PERSPECTIVE" };
	static int item_current_idx = (UINT)GetTargetObject()->Camera()->GetProjType();
	const char* combo_preview_value = Projitems[item_current_idx];

	ImGui::Text("Proj Type  ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##ProjTypeCombo", combo_preview_value))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Projitems); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(Projitems[n], is_selected))
			{
				item_current_idx = n;
				SelectType(Projitems[item_current_idx]);
			}

			// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// 투영 최대 거리 수정
	float fFar = GetTargetObject()->Camera()->GetFar();
	ImGui::Text("Far        ");
	ImGui::SameLine(); 
	ImGui::DragFloat("##Camera Far", &fFar);
	GetTargetObject()->Camera()->SetFar(fFar);

	// FOV 수정 (원근)
	float fFOV = GetTargetObject()->Camera()->GetFOV();
	ImGui::Text("FOV        ");			// 원근투영 - 시야각
	ImGui::SameLine();
	ImGui::BeginDisabled(!(1 == item_current_idx));
	ImGui::DragFloat("##Camera FOV", &fFOV);
	ImGui::EndDisabled();
	GetTargetObject()->Camera()->SetFOV(fFOV);

	// Scale 수정
	// 직교 min 0.1 ~ max 13.0
	float fScale = GetTargetObject()->Camera()->GetScale();
	ImGui::Text("Scale      ");
	ImGui::SameLine();
	ImGui::BeginDisabled(!(0 == item_current_idx));
	ImGui::DragFloat("##Camera Scale", &fScale, 0.005f, 0.1f, 13.0f, "%.3f");
	ImGui::EndDisabled();
	GetTargetObject()->Camera()->SetScale(fScale);


	// Layer 선택시 해당 레이어만 렌더
	m_strVec.push_back("All Layers");
	CLevelMgr::GetInst()->GetCurrentLevel()->GetLayerName(m_strVec);

	static int Layer_current_idx = 0;
	const char* combo_preview_value2 = m_strVec[Layer_current_idx].c_str();

	ImGui::Text("Layer      ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##LayerCombo", combo_preview_value2))
	{
		for (int n = 0; n < m_strVec.size(); n++)
		{
			const bool is_selected = (Layer_current_idx == n);
			if (ImGui::Selectable(m_strVec[n].c_str(), is_selected))
			{
				Layer_current_idx = n;
				SelectLayer(m_strVec[Layer_current_idx]);
			}
		}
		ImGui::EndCombo();
	}
	m_strVec.clear();

		// ImGui::Text("Camera Idx  ");		// camera idx
}

void CameraUI::SelectType(const string _str)
{
	if ("ORTHOGRAPHIC" == _str)
		GetTargetObject()->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	if ("PERSPECTIVE" == _str)
		GetTargetObject()->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
}

void CameraUI::SelectLayer(const string _str)
{
	if ("All Layers" == _str)
	{
		GetTargetObject()->Camera()->LayerCheckAll();
		GetTargetObject()->Camera()->LayerCheck(L"UI", false);
	}
	else
	{
		GetTargetObject()->Camera()->LayerCheckOff();
		GetTargetObject()->Camera()->LayerCheck(ToWString(_str), true);
	}
}