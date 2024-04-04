#include "pch.h"
#include "PrefabUI.h"

#include "Inspector.h"
#include <Scripts/CScriptMgr.h>

PrefabUI::PrefabUI()
	: AssetUI("Prefab", "##Prefab", ASSET_TYPE::PREFAB)
	, m_ViewObj(nullptr)
{
}

PrefabUI::~PrefabUI()
{
	//Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
//	pInspector->GetTargetAsset();
}

void PrefabUI::render_update()
{
	AssetUI::render_update();

	Ptr<CPrefab> pPrefab = (CPrefab*)GetAsset().Get();
	string strPath = string(pPrefab->GetRelativePath().begin(), pPrefab->GetRelativePath().end());

	ImGui::Text((char*)strPath.c_str());
	ImGui::Separator();

	m_ViewObj = pPrefab->GetProtoGameObj();
	string strName = ToString(m_ViewObj->GetName());
	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::InputText("##ObjName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);


	if (ImGui::TreeNode("Component"))
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			int type = i;
			switch (type)
			{
			case (UINT)COMPONENT_TYPE::TRANSFORM:
				if (nullptr != m_ViewObj->Transform()) ImGui::Text("Transform");
				break;
			case (UINT)COMPONENT_TYPE::COLLIDER2D:
				if (nullptr != m_ViewObj->Collider2D()) ImGui::Text("Collider2D");
				break;
			case (UINT)COMPONENT_TYPE::MOVEMENT:
				if (nullptr != m_ViewObj->Movement()) ImGui::Text("Movement");
				break;
			case (UINT)COMPONENT_TYPE::ANIMATOR2D:
				if (nullptr != m_ViewObj->Animator2D()) ImGui::Text("Anmiator2D");
				break;
			case (UINT)COMPONENT_TYPE::LIGHT2D:
				if (nullptr != m_ViewObj->Light2D()) ImGui::Text("Light2D");
				break;
			case (UINT)COMPONENT_TYPE::CAMERA:
				if (nullptr != m_ViewObj->Camera()) ImGui::Text("Camera");
				break;
			case (UINT)COMPONENT_TYPE::STATEMACHINE:
				if (nullptr != m_ViewObj->StateMachine()) ImGui::Text("StateMachine");
				break;
			case (UINT)COMPONENT_TYPE::MESHRENDER:
				if (nullptr != m_ViewObj->MeshRender()) ImGui::Text("MeshRender");
				break;
			case (UINT)COMPONENT_TYPE::TILEMAP:
				if (nullptr != m_ViewObj->TileMap()) ImGui::Text("TileMap");
				break;
			case (UINT)COMPONENT_TYPE::END:
				break;
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Script"))
	{
		vector<CScript*> scripts = m_ViewObj->GetScripts();
		for (size_t i = 0; i < scripts.size(); ++i)
		{
			string strName = ToString(CScriptMgr::GetScriptName(scripts[i]));
			ImGui::Text((char*)strName.c_str());
		}

		ImGui::TreePop();
	}

	if (ImGui::Button("Add Level"))
	{
		CGameObject* pNewObj = pPrefab->Instantiate();
		GamePlayStatic::SpawnGameObject(pNewObj, 0);
	}

	ImGui::SameLine();
	ImGui::Text(" LayerIdx Default");



}