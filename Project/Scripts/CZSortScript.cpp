#include "pch.h"
#include "CZSortScript.h"

CZSortScript::CZSortScript()
	: CScript(ZSORTSCRIPT)
{
}

CZSortScript::~CZSortScript()
{
}


void CZSortScript::begin()
{
}

void CZSortScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	// y위치에 따른 z축 정렬
	float limity = 5000.f;
	float limitz = 1500.f;
	if (0 <= vPos.y)
		vPos.z = (1.f / limity) * 1500;

	if (4 == GetOwner()->GetLayerIdx())
		vPos.z = (vPos.y / limity) * 1500 + 50.f;
	else
		vPos.z = (vPos.y / limity) * 1500;

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
}

void CZSortScript::SaveToFile(FILE* _File)
{
}

void CZSortScript::LoadFromFile(FILE* _File)
{
}

