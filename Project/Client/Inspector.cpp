#include "pch.h"
#include "Inspector.h"

#include <Engine/CTransform.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CTaskMgr.h>

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
		static char sName[256] = {};
		if (ImGui::Button("New Name"))
		{
			m_TargetObject->SetName(ToWString((string)sName));
			sName[0] = { 0 };
			CTaskMgr::GetInst()->IsFixObject(true);
		}
		ImGui::SameLine();
		ImGui::InputText("##InputTexName", (char*)sName, IM_ARRAYSIZE(sName));

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
	ImGui::PushItemWidth(150);
	vector<string> layers;
	CLevelMgr::GetInst()->GetCurrentLevel()->GetLayerName(layers);
	const char* combo_preview = "Select";
	static const char* left_value = combo_preview;
	static const char* right_value = combo_preview;


	static int left_idx = -1;

	if (-1 == left_idx)
		left_value = combo_preview;
	else
		left_value = layers[left_idx].c_str();

	if (ImGui::BeginCombo("##LeftLayer", left_value))
	{
		for (int n = 0; n < layers.size(); n++)
		{
			if ("\0" == layers[n])
				continue;

			const bool is_selected = (left_idx == n);
			if (ImGui::Selectable(layers[n].c_str(), is_selected))
			{
				left_idx = n;
			}

			// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

		ImGui::SameLine();
		static int right_idx = -1;

		if (-1 == right_idx)
			right_value = combo_preview;
		else
			right_value = layers[right_idx].c_str();

		if (ImGui::BeginCombo("##RightLayer", right_value))
		{
			for (int n = 0; n < layers.size(); n++)
			{
				if ("\0" == layers[n])
					continue;

				const bool is_selected = (right_idx == n);
				if (ImGui::Selectable(layers[n].c_str(), is_selected))
				{
					right_idx = n;
				}

				// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		if (ImGui::Button("Apply"))
		{
			CCollisionMgr::GetInst()->LayerCheck(left_idx, right_idx);
			left_idx = -1;
			right_idx = -1;
		}
}