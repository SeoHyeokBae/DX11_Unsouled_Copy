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
	// 1. LightType�� DIRECTIONAL �̸� 
	//		- vAmbient����
	// 2. DIRECTIONAL�� �ƴϸ� 
	//		- Color 
	//		- vWorldPos
	//		- vWorldDir	(POINT�� �����)
	//		- fRadius
	//		- fAngle	(POINT�� �����)
	//		- LightType����
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
		// LightType == DIRECTIONAL �϶� �ּ� ���� ����
		ImGui::Text("Ambient   ");
		//ToDo
	}
	else
	{
		// Position ����
		ImGui::Text("Offset Pos   ");
		ImGui::SameLine(); ImGui::DragFloat3("##Light2D OffsetPos", vOffsetPos);
		GetTargetObject()->Light2D()->SetOffset(vOffsetPos);

		// Spot�϶� Light ���� ����
		if (2 == LightTypeEnum)
		{
			ImGui::Text("Dir         ");
			//ToDo
		}

		// ������(ũ��) ���� 
		float fRadius = TargetInfo.fRadius;
		ImGui::Text("Radius       ");
		ImGui::SameLine(); ImGui::DragFloat("##Light2D Radius", &fRadius);
		GetTargetObject()->Light2D()->SetRadius(fRadius);

		// Spot�϶� ���� ���� (�ּ� 0 ~ �ִ� 360)
		if (2 == LightTypeEnum)
		{
			static float slider_f = TargetInfo.fAngle;

			ImGui::Text("Angle        "); ImGui::SameLine();
			ImGui::SliderFloat("##Light2D Angle", &slider_f, 0.0f, 360.0f, "%.1f degree");
			GetTargetObject()->Light2D()->SetAngle(slider_f);
		}

		// ����Ʈ ���� ����
		ImGui::Text("Color        ");
		//ToDo


	}
}


//Vec4	vColor;		// ���� ����	- ������ ���� ����
//Vec4	vSpecular;	// �ݻ籤	- ������ ��ü ǥ���� �ݻ�� ����
//Vec4	vAmbient;	// ȯ�汤(�ֺ���) - ������ ���ؼ� ����Ǵ� �ּ����� ��
//
//Vec3	vWorldPos;	// ������ ��ġ
//Vec3	vWorldDir;	// ������ ���ϴ� ����
//float	fRadius;	// ������ ���� �ݰ�, �Ÿ� ����
//float	fAngle;		// ������ ���� ����
//
//int		LightType;	// ���� Ÿ��
