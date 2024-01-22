#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("Tarnsform", "##Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComponentTitle("Transform");
}


TransformUI::~TransformUI()
{
}

void TransformUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	Vec3 vPos = GetTargetObject()->Transform()->GetRelativePos();
	Vec3 vScale = GetTargetObject()->Transform()->GetRelativeScale();
	Vec3 vRot = GetTargetObject()->Transform()->GetRelativeRotation();
	vRot.ToDegree();
	
	ImGui::Text("Position"); 
	ImGui::SameLine(); ImGui::DragFloat3("##Relative Position", vPos);

	ImGui::Text("Scale   "); 
	ImGui::SameLine(); ImGui::DragFloat3("##Relative Scale", vScale);

	ImGui::Text("Rotation"); 
	ImGui::SameLine(); ImGui::DragFloat3("##Relative Rotation", vRot);

	// Input시 수정 가능하게
	vRot.ToRadian();
	GetTargetObject()->Transform()->SetRelativePos(vPos);
	GetTargetObject()->Transform()->SetRelativeScale(vScale);
	GetTargetObject()->Transform()->SetRelativeRotation(vRot);

	// Absolute값
	bool bAbsolute = GetTargetObject()->Transform()->IsAbsolute();
	ImGui::Text("Scale Absolute"); ImGui::SameLine(); ImGui::Checkbox("##TransformAbsolute", &bAbsolute);
	GetTargetObject()->Transform()->SetAbsolute(bAbsolute);

}
