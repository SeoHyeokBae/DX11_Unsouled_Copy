#include "pch.h"
#include "Animator2DUI.h"
#include <Engine/CAnimator2D.h>
#include <Engine/CAnim.h>
#include "ListUI.h"
#include "AnimationEditorUI.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D","##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComponentTitle("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	CGameObject* pTarget = GetTargetObject();
	//CAnim* pAnim = pTarget->Animator2D()->GetCurAnim();

	string animname = ToString(pTarget->Animator2D()->GetCurKey().c_str());

	ImGui::Text("Current Animation ");
	ImGui::SameLine();
	//ImGui::InputText("##Current Animation", (char*)animname.c_str(), animname.length(), ImGuiInputTextFlags_ReadOnly);
	if (ImGui::Button((char*)animname.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 20.f, 20)))
	{
		// 리스트 UI
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##List");

		vector<string> vecAnimation;
		pTarget->Animator2D()->GetAnimName(vecAnimation);
		pListUI->AddString(vecAnimation);
		pListUI->SetDbClickDelegate(this, (Delegate_1)&Animator2DUI::AnimSelect);
		pListUI->Activate();

		//pListUI->AddString(vecMeshName);
		////pListUI->SetDbClickCallBack(MeshSelect);
		//pListUI->SetDbClickDelegate(this, (Delegate_1)&MeshRenderUI::MeshSelect);
		//pListUI->Activate();
	}

	if (ImGui::Button("Editor"))
	{
		// EditorUI
		AnimationEditorUI* Editor = (AnimationEditorUI*)CImGuiMgr::GetInst()->FindUI("##AnimationEditor");

		Editor->Activate();
	}



	// 컷팅
	//// Normalized coordinates of pixel (10,10) in a 256x256 texture.
	//ImVec2 uv0 = ImVec2(0.0f / my_image_width, 0.0f / my_image_height);

	//// Normalized coordinates of pixel (110,210) in a 256x256 texture.
	//ImVec2 uv1 = ImVec2((0.0f + 120.0f*0.3f) / my_image_width, (0.0f + 130.0f*0.3f) / my_image_height);

	//ImGui::Text("uv0 = (%f, %f)", uv0.x, uv0.y);
	//ImGui::Text("uv1 = (%f, %f)", uv1.x, uv1.y);

	//ImGui::Begin("DirectX11 Texture Test2");

	//ImGui::Text("pointer = %p", my_texture);
	//ImGui::Text("size = %d x %d", my_image_width, my_image_height);
	//ImGui::Image((void*)my_texture.Get(), ImVec2(my_image_width, my_image_height),uv0,uv1);
	//ImGui::End();
}

void Animator2DUI::AnimSelect(DWORD_PTR _ptr)
{
	string strAnim = (char*)_ptr;
	wstring strAnimName = ToWString(strAnim);

	CAnim* pAnim = GetTargetObject()->Animator2D()->FindAnim(strAnimName);
	GetTargetObject()->Animator2D()->SetCurAnim(pAnim);
}



//static bool open = true;
//if (ImGui::CollapsingHeader("Header with a close button", &open))
//{
//	ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
//	for (int i = 0; i < 5; i++)
//		ImGui::Text("More content %d", i);
//}


// 이미지에서 특정 픽셀만 컷팅
//// Normalized coordinates of pixel (10,10) in a 256x256 texture.
//ImVec2 uv0 = ImVec2(10.0f / 256.0f, 10.0f / 256.0f);
//
//// Normalized coordinates of pixel (110,210) in a 256x256 texture.
//ImVec2 uv1 = ImVec2((10.0f + 100.0f) / 256.0f, (10.0f + 200.0f) / 256.0f);
//
//ImGui::Text("uv0 = (%f, %f)", uv0.x, uv0.y);
//ImGui::Text("uv1 = (%f, %f)", uv1.x, uv1.y);
//
//// Display the 100x200 section starting at (10,10)
//ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(100.0f, 200.0f), uv0, uv1);




//이름이 지정된 변수를 사용하여 다르게 작성된 동일한 코드 :
//
//ImVec2 display_min = ImVec2(10.0f, 10.0f);		// LeftTop
//ImVec2 display_size = ImVec2(100.0f, 200.0f);		// RightBottom
//ImVec2 texture_size = ImVec2(256.0f, 256.0f);
//
//// Normalized coordinates of pixel (10,10) in a 256x256 texture.
//ImVec2 uv0 = ImVec2(display_min.x / texture_size.x, display_min.y / texture_size.y);
//
//// Normalized coordinates of pixel (110,210) in a 256x256 texture.
//ImVec2 uv1 = ImVec2((display_min.x + display_size.x) / texture_size.x, (display_min.y + display_size.y) / texture_size.y);
//
//ImGui::Text("uv0 = (%f, %f)", uv0.x, uv0.y);
//ImGui::Text("uv1 = (%f, %f)", uv1.x, uv1.y);
//
//// Display the 100x200 section starting at (10,10)
//ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(display_size.x, display_size.y), uv0, uv1);