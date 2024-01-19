#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMeshRender.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"


void MeshSelect(DWORD_PTR _ptr);
void MaterialSelect(DWORD_PTR _ptr);


MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRender", "##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetSize(ImVec2(0.f, 100.f));
	SetComponentTitle("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTarget = GetTargetObject();
	CMeshRender* pMeshRender = pTarget->MeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();

	string meshname = ToString(pMesh->GetKey()).c_str();
	string mtrlname = ToString(pMtrl->GetKey()).c_str();

	ImGui::Text("Mesh    ");
	ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)meshname.c_str(), meshname.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(20, 20)))
	{
		// 리스트 UI
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMeshName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MESH, vecMeshName);

		pListUI->AddString(vecMeshName);
		pListUI->SetDbClickCallBack(MeshSelect);
		pListUI->Activate();
	}

	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::InputText("##MtrlName", (char*)mtrlname.c_str(), mtrlname.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(20, 20)))
	{
		// 리스트 UI
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecMtrlName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMtrlName);

		pListUI->AddString(vecMtrlName);
		pListUI->SetDbClickCallBack(MaterialSelect);
		pListUI->Activate();
	}
}

void MeshSelect(DWORD_PTR _ptr)
{
	string strMesh = (char*)_ptr;
	wstring strMeshName = ToWString(strMesh);

	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshName);

	Inspector* pInstector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	CGameObject* pTargetObject = pInstector->GetTargetObject();

	pTargetObject->MeshRender()->SetMesh(pMesh);
}

void MaterialSelect(DWORD_PTR _ptr)
{
	string strMtrl = (char*)_ptr;
	wstring strMtrlName = ToWString(strMtrl);

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMtrlName);

	Inspector* pInstector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	CGameObject* pTargetObject = pInstector->GetTargetObject();

	pTargetObject->MeshRender()->SetMaterial(pMtrl);
}