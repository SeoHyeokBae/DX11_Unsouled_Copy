#include "pch.h"
#include "Inspector.h"

#include <Engine/CTransform.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "Animator2DUI.h"
#include "ScriptUI.h"
#include "AssetUI.h"

#include "CameraUI.h"


Inspector::Inspector()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_arrComUI{}
{
	// 자식 UI 생성
	CreateChildUI();
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

	if (nullptr != m_TargetObject)
	{
		string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
		ImGui::Text(strName.c_str());
		ImGui::Separator();
		string levelName = ToString(CLevelMgr::GetInst()->GetCurrentLevel()->GetName());
		ImGui::Text(levelName.c_str());
		ImGui::Text("Layer Idx");
		ImGui::SameLine();

		// 레이어 선택
		vector<string> layers;
		CLevelMgr::GetInst()->GetCurrentLevel()->GetLayerName(layers);

		int cur_idx = GetTargetObject()->GetLayerIdx();
		const char* preview_value = layers[cur_idx].c_str();
		
		if (ImGui::BeginCombo("##ObjLayer", preview_value))
		{
			for (int n = 0; n < layers.size(); n++)
			{
				if ("\0" == layers[n])
					continue;

				const bool is_selected = (cur_idx == n);
				if (ImGui::Selectable(layers[n].c_str(), is_selected))
				{
					cur_idx = n;
					CLayer* layer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(GetTargetObject()->GetLayerIdx());
					layer->DetachGameObject(GetTargetObject());
					layer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(cur_idx);
					layer->AddObject(GetTargetObject(), false);
				}

				// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::Text("         ");
		ImGui::SameLine();
		if (ImGui::TreeNode("Layer Conflict Check"))
		{
			CreateLayerCheck();
			ImGui::TreePop();
		}

	}
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	// Target 오브젝트 설정
	m_TargetObject = _Object;

	// 해당 오브젝트가 보유하고 있는 컴포넌트에 대응하는 컴포넌트UI 활성화
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			m_arrComUI[i]->SetTargetObject(_Object);
		}
	}

	// 해당 오브젝트가 보유하고 있는 Script 에 맞추어서 ScriptUI 를 활성화 시킨다.
	if (nullptr == _Object)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Deactivate();
		}
	}
	else
	{
		if (m_vecScriptUI.size() < _Object->GetScripts().size())
		{
			ResizeScriptUI(_Object->GetScripts().size());
		}

		const vector<CScript*>& vecScripts = _Object->GetScripts();
		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(vecScripts[i]);
		}
	}

	// AssetUI 비활성화
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	// 이전에 선택된 오브젝트 비워줌
	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
	{
		m_arrAssetUI[i]->Deactivate();
	}

	if (nullptr != m_TargetAsset)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->Activate();
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->SetAsset(_Asset);
	}
}


void Inspector::CreateLayerCheck()
{
	vector<string> layers;
	CLevelMgr::GetInst()->GetCurrentLevel()->GetLayerName(layers);

	vector<string> newlayers;
	for (int n = 0; n < layers.size(); n++)
	{
		if ("\0" == layers[n])
			continue;
		newlayers.push_back(layers[n]);
	}

	const int columns_count = LAYER_MAX;
	const int rows_count = LAYER_MAX;

	static ImGuiTableFlags table_flags 
		= ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable  | ImGuiTableFlags_HighlightHoveredColumn;
	
	static bool bools[columns_count * rows_count];
	vector<int> input;
	if (ImGui::BeginTable("Layer Check", columns_count, table_flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * newlayers.size())))
	{
		ImGui::TableSetupColumn("Layer", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
		for (int n = 1; n < columns_count; n++)
		{ 
			if ("\0" == layers[n])
				continue;

			input.push_back(n);
			char col[50] = {};
			sprintf_s(col, "%d", n);
			ImGui::TableSetupColumn(col, ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed , 20.f);
		}
		ImGui::TableSetupScrollFreeze(1, 1);

		ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
		ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
		for (int row = 1; row < rows_count; row++)
		{
			if ("\0" == layers[row])
				continue;

			ImGui::PushID(row);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::Text(layers[row].c_str());
			int count = 0;
			for (int column = 1; column <= input.size(); column++)
				if (ImGui::TableSetColumnIndex(column))
				{
					ImGui::PushID(column);
					ImGui::Checkbox("", &bools[row * columns_count + input[column-1]]);
					ImGui::PopID();
				}
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}