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
	ImGui::Text("Object");
	ImGui::SameLine();
	ImGui::InputText("##ObjName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);


	//for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	//{
	//	if (nullptr != m_ViewObj->GetComponent(COMPONENT_TYPE::))
	//	{
	//		m_arrComUI[i]->SetTargetObject(_Object);
	//	}
	//}

	//// 해당 오브젝트가 보유하고 있는 Script 에 맞추어서 ScriptUI 를 활성화 시킨다.

	if (ImGui::TreeNode("Scripts"))
	{
		vector<wstring> vecScriptName;
		CScriptMgr::GetScriptInfo(vecScriptName);

		for (size_t i = 0; i < vecScriptName.size(); ++i)
		{
			string strName = ToString(vecScriptName[i]);
			ImGui::Text((char*)strName.c_str());
		}

		ImGui::TreePop();
	}

	if (ImGui::Button("Add Level"))
	{
		// ToDo
		
		
	}



}