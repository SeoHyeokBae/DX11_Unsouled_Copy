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

	// y��ġ�� ���� z�� ����
	float limity = 5000.f;
	float limitz = 1500.f;
	if (0 <= vPos.y)
		vPos.z = (1.f / limity) * 1500;

	if (5 == GetOwner()->GetLayerIdx()) // �׸���
	{
		vPos.z = (vPos.y / limity) * 1500.f + 2.f;
	}
	else if (3 == GetOwner()->GetLayerIdx()) // ����
	{
		vPos.z = (vPos.y / limity) * 1500.f + 3.f;
	}
	else
	{
		vPos.z = (vPos.y / limity) * 1500.f;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
}

void CZSortScript::SaveToFile(FILE* _File)
{
}

void CZSortScript::LoadFromFile(FILE* _File)
{
}

