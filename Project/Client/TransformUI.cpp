#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: UI("Tarnsform", "##Transform")
	, m_TargetObject(nullptr)
{
}


TransformUI::~TransformUI()
{
}

void TransformUI::render_update()
{
	if (nullptr == m_TargetObject)
		return;

	Vec3 vPos = m_TargetObject->Transform()->GetRelativePos();
	Vec3 vScale = m_TargetObject->Transform()->GetRelativeScale();
	Vec3 vRot = m_TargetObject->Transform()->GetRelativeRotation();
	vRot.ToDegree();

	ImGui::InputFloat3("Relative Position", vPos);
	ImGui::InputFloat3("Relative Scale", vScale);
	ImGui::InputFloat3("Relative Rotation", vRot);

	// Input시 수정 가능하게
	vRot.ToRadian();
	m_TargetObject->Transform()->SetRelativePos(vPos);
	m_TargetObject->Transform()->SetRelativeScale(vScale);
	m_TargetObject->Transform()->SetRelativeRotation(vRot);
}
