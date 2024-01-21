#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0.f, 150.f));
	SetComponentTitle("Light2D");
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::render_update()
{
	ComponentUI::render_update();

	//ToDo
	// 1. LightType이 DIRECTIONAL 이면 
	//		- vAmbient색깔만
	// 2. DIRECTIONAL이 아니면 
	//		- Color 
	//		- vWorldPos
	//		- vWorldDir	(POINT면 미출력)
	//		- fRadius
	//		- fAngle	(POINT면 미출력)
	//		- LightType변경
	tLightInfo TargetInfo = GetTargetObject()->Light2D()->GetLightInfo();

	static int LightTypeEnum = (UINT)TargetInfo.LightType;
	
	ImGui::Text("LightType    ");ImGui::SameLine();
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
		ImGui::Text("Ambient   ");
		//ToDo
	}
	else
	{
		// Position 수정
		ImGui::Text("Offset Pos   ");
		ImGui::SameLine(); ImGui::DragFloat3("##Light2D OffsetPos", vOffsetPos);
		GetTargetObject()->Light2D()->SetOffset(vOffsetPos);

		// Spot일때 Light 방향 수정
		if (2 == LightTypeEnum)
		{
			ImGui::Text("Dir         ");
			//ToDo
		}

		// 반지름(크기) 수정 
		float fRadius = TargetInfo.fRadius;
		ImGui::Text("Radius       ");
		ImGui::SameLine(); ImGui::DragFloat("##Light2D Radius", &fRadius);
		GetTargetObject()->Light2D()->SetRadius(fRadius);

		// Spot일때 각도 수정 (최소 0 ~ 최대 360)
		if (2 == LightTypeEnum)
		{
			static float slider_f = TargetInfo.fAngle;

			ImGui::Text("Angle        "); ImGui::SameLine();
			ImGui::SliderFloat("##Light2D Angle", &slider_f, 0.0f, 360.0f, "%.1f degree");
			GetTargetObject()->Light2D()->SetAngle(slider_f);
		}

		// 라이트 색상 수정
		ImGui::Text("Color        ");
		//ToDo


	}
}


//Vec4	vColor;		// 빛의 색상	- 광원의 순수 색상
//Vec4	vSpecular;	// 반사광	- 광원이 물체 표변에 반사된 색상
//Vec4	vAmbient;	// 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛
//
//Vec3	vWorldPos;	// 광원의 위치
//Vec3	vWorldDir;	// 광원이 향하는 방향
//float	fRadius;	// 광원의 영향 반경, 거리 정보
//float	fAngle;		// 광원의 범위 각도
//
//int		LightType;	// 광원 타입
