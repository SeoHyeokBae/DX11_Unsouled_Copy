#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>


Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", "##Collider2D", COMPONENT_TYPE::COLLIDER2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComponentTitle("Collider2D");
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	Vec2 vOffsetPos = GetTargetObject()->Collider2D()->GetOffsetPos();
	Vec2 vOffsetScale = GetTargetObject()->Collider2D()->GetOffsetScale();

	ImGui::Text("Offset Pos   ");
	ImGui::SameLine(); ImGui::DragFloat2("##Collider2D OffsetPos", vOffsetPos);

	ImGui::Text("Offset Scale ");
	ImGui::SameLine(); ImGui::DragFloat2("##Collider2D OffsetScale", vOffsetScale);

	// Input시 수정 가능하게
	GetTargetObject()->Collider2D()->SetOffsetPos(vOffsetPos);
	GetTargetObject()->Collider2D()->SetOffsetScale(vOffsetScale);
	
	// ColliderType 선택
	const char* items[] = { "RECT" , "CIRCLE" };
	static int item_current_idx = 0;
	const char* combo_preview_value = items[item_current_idx];

	ImGui::Text("Select Type  ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##ColliderTypeCombo", combo_preview_value))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
			{
				item_current_idx = n;
				SelectType(items[item_current_idx]);
			}

			// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// Collider Render 여부
	bool bShow = GetTargetObject()->Collider2D()->IsVisible();
	ImGui::Text("Show Collider"); ImGui::SameLine(); ImGui::Checkbox("##ShowCollider", &bShow);
	GetTargetObject()->Collider2D()->SetVisible(bShow);

	// Absolute값
	ImGui::SameLine();
	bool bAbsolute = GetTargetObject()->Collider2D()->IsAbsolute();
	ImGui::Text("Scale Absolute"); ImGui::SameLine(); ImGui::Checkbox("##ColliderAbsolute", &bAbsolute);
	GetTargetObject()->Collider2D()->SetAbsolute(bAbsolute);

}

void Collider2DUI::SelectType(const string _str)
{
	if ("RECT" == _str)
		GetTargetObject()->Collider2D()->SetColliderType(COLLIDER2D_TYPE::RECT);
	
	if ("CIRCLE" == _str)
		GetTargetObject()->Collider2D()->SetColliderType(COLLIDER2D_TYPE::CIRCLE);
}