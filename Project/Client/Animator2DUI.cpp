#include "pch.h"
#include "Animator2DUI.h"
#include <Engine/components.h>
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
	if (nullptr != GetTargetObject()->Animator2D()->GetCurAnim() && !GetTargetObject()->IsShadow()) // 임시조치
		if (ImGui::Button((char*)animname.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 20.f, 20)))
		{
			// 리스트 UI
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("Select##List");

			vector<string> vecAnimation;
			pTarget->Animator2D()->GetAnimName(vecAnimation);
			pListUI->AddString(vecAnimation);
			pListUI->SetDbClickDelegate(this, (Delegate_1)&Animator2DUI::AnimSelect);
			pListUI->Activate();
		}


	if (ImGui::Button("Editor"))
	{
		// EditorUI
		AnimationEditorUI* Editor = (AnimationEditorUI*)CImGuiMgr::GetInst()->FindUI("##AnimationEditor");
		Editor->SetGameObj(GetTargetObject());
		Editor->Activate();
	}
}

void Animator2DUI::AnimSelect(DWORD_PTR _ptr)
{
	string strAnim = (char*)_ptr;
	wstring strAnimName = ToWString(strAnim);

	CAnim* pAnim = GetTargetObject()->Animator2D()->FindAnim(strAnimName);
	GetTargetObject()->Animator2D()->SetCurAnim(pAnim);
	GetTargetObject()->Animator2D()->Play(pAnim->GetName(), true);
}