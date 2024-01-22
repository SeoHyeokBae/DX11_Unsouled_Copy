#include "pch.h"
#include "Animator2DUI.h"

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
}
