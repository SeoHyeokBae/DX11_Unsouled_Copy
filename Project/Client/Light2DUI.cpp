#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0.f, 170.f));
	SetComponentTitle("Light2D");
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::render_update()
{
	ComponentUI::render_update();

	tLightInfo TargetInfo = GetTargetObject()->Light2D()->GetLightInfo();
	static int LightTypeEnum = (UINT)TargetInfo.LightType;
	
	ImGui::Text("LightType  ");ImGui::SameLine();
	if (ImGui::RadioButton("POINT", &LightTypeEnum, 1))
		GetTargetObject()->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	
	ImGui::SameLine();
	if (ImGui::RadioButton("SPOT", &LightTypeEnum, 2))
		GetTargetObject()->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	
	ImGui::SameLine();
	if (ImGui::RadioButton("DIRECTIONAL", &LightTypeEnum, 0))
		GetTargetObject()->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);


	Vec3 vOffsetPos = GetTargetObject()->Light2D()->GetOffsetPos();

	if (0 == LightTypeEnum)
	{
		// LightType == DIRECTIONAL 일때 최소 보장 색상
		ImGui::Text("Ambient    "); ImGui::SameLine();
		static ImVec4 color = ImVec4(TargetInfo.vAmbient.x, TargetInfo.vAmbient.y, TargetInfo.vAmbient.z,0.f);
		ImGui::ColorEdit3("##AmbientColor", (float*)&color);
		GetTargetObject()->Light2D()->SetAmbient(Vec3(color.x, color.y, color.z));
	}

	else
	{
		// Position 수정
		ImGui::Text("Offset Pos ");
		ImGui::SameLine(); ImGui::DragFloat3("##Light2D OffsetPos", vOffsetPos);
		GetTargetObject()->Light2D()->SetOffset(vOffsetPos);

		// Spot일때 Light 방향 수정
		if (2 == LightTypeEnum)
		{
			ImGui::Text("Dir        ");
			ImGui::SameLine();
			enum Element { RIGHT,UP,LEFT,DOWN ,END};
			static int elem = (int)GetTargetObject()->Light2D()->GetLightDir();
			const char* elems_names[END] = { "RIGHT" , "UP", "LEFT", "DOWN" };
			const char* elem_name = (elem >= 0 && elem < END) ? elems_names[elem] : "Unknown";
			ImGui::SliderInt("##Dir enum", &elem, 0, END - 1, elem_name);
			SelectDir(elems_names[elem]);

		}
		// 반지름(크기) 수정 
		float fRadius = TargetInfo.fRadius;
		ImGui::Text("Radius     ");
		ImGui::SameLine(); ImGui::DragFloat("##Light2D Radius", &fRadius);
		GetTargetObject()->Light2D()->SetRadius(fRadius);

		// Spot일때 각도 수정 (최소 0 ~ 최대 360)
		if (2 == LightTypeEnum)
		{
			static float slider_f = TargetInfo.fAngle;

			ImGui::Text("Angle      "); ImGui::SameLine();
			ImGui::SliderFloat("##Light2D Angle", &slider_f, 0.0f, 360.0f, "%.1f degree");
			GetTargetObject()->Light2D()->SetAngle(slider_f);
		}

		// 라이트 색상 수정
		ImGui::Text("LightColor "); ImGui::SameLine();
		static ImVec4 lightcolor = ImVec4(TargetInfo.vColor.x, TargetInfo.vColor.y, TargetInfo.vColor.z, 0.f);
		ImGui::ColorEdit3("##LightColor", (float*)&lightcolor);
		GetTargetObject()->Light2D()->SetLightColor(Vec3(lightcolor.x, lightcolor.y, lightcolor.z));
	}
}

void Light2DUI::SelectDir(const string _str)
{
	if ("RIGHT" == _str)
		GetTargetObject()->Light2D()->SetLightDir(eLight2DDir::RIGHT);
	else if ("LEFT" == _str)
		GetTargetObject()->Light2D()->SetLightDir(eLight2DDir::LEFT);
	else if ("UP" == _str)
		GetTargetObject()->Light2D()->SetLightDir(eLight2DDir::UP);
	else if ("DOWN" == _str)
		GetTargetObject()->Light2D()->SetLightDir(eLight2DDir::DOWN);
}
