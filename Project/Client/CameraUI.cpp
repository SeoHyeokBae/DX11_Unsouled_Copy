#include "pch.h"
#include "CameraUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>

CameraUI::CameraUI()
	: ComponentUI("Camera","##Camera", COMPONENT_TYPE::CAMERA)
{
	SetSize(ImVec2(0.f, 450.f));
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

	// ���� ����
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

			// ����Ʈ �� �ش� �׸��� Ŭ���Ǹ� ���̶���Ʈ �ɾ���
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// ���� �ִ� �Ÿ� ����
	float fFar = GetTargetObject()->Camera()->GetFar();
	ImGui::Text("Far        ");
	ImGui::SameLine(); 
	ImGui::DragFloat("##Camera Far", &fFar);
	GetTargetObject()->Camera()->SetFar(fFar);

	// FOV ���� (����)
	float fFOV = GetTargetObject()->Camera()->GetFOV();
	ImGui::Text("FOV        ");			// �������� - �þ߰�
	ImGui::SameLine();
	ImGui::BeginDisabled(!(1 == item_current_idx));
	ImGui::DragFloat("##Camera FOV", &fFOV);
	ImGui::EndDisabled();
	GetTargetObject()->Camera()->SetFOV(fFOV);

	// Scale ����
	// ���� min 0.1 ~ max 13.0
	float fScale = GetTargetObject()->Camera()->GetScale();
	ImGui::Text("Scale      ");
	ImGui::SameLine();
	ImGui::BeginDisabled(!(0 == item_current_idx));
	ImGui::DragFloat("##Camera Scale", &fScale, 0.005f, 0.1f, 13.0f, "%.3f");
	ImGui::EndDisabled();
	GetTargetObject()->Camera()->SetScale(fScale);


	// Layer ���ý� �ش� ���̾ ����
	//m_strVec.push_back("All Layers");
	vector<string> layers;
	CLevelMgr::GetInst()->GetCurrentLevel()->GetLayerName(layers);

	if (ImGui::TreeNode("Layers"))
	{
		UINT check = GetTargetObject()->Camera()->GetLayerCheck();
		vector<bool> selection;
		for (size_t i = 0; i < layers.size(); i++)
		{
			selection.push_back(check & (1 << i));
		}

		for (size_t i = 0; i < layers.size(); i++)
		{
			if ("\0" == layers[i])
				continue;

			if (ImGui::Selectable(layers[i].c_str(), selection[i], ImGuiSelectableFlags_AllowDoubleClick))
				if (ImGui::IsMouseDoubleClicked(0))
				{
					selection[i] = !selection[i];
					GetTargetObject()->Camera()->LayerCheck(ToWString(layers[i]), selection[i]);
				}
		}
		ImGui::TreePop();

		selection.clear();
		layers.clear();
	}
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

// Selectabled basic