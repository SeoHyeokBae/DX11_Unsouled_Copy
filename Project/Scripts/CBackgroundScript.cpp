#include "pch.h"
#include "CBackgroundScript.h"

#include <Engine/CKeyMgr.h>

CBackgroundScript::CBackgroundScript()
	: CScript(BACKGROUNDSCRIPT)
{
}

CBackgroundScript::~CBackgroundScript()
{
}


void CBackgroundScript::tick()
{

	static int i = 0;
	if (KEY_TAP(KEY::T))
	{
		i ? MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1) : MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		i = !i;
	}
}

void CBackgroundScript::SaveToFile(FILE* _File)
{
}

void CBackgroundScript::LoadFromFile(FILE* _File)
{
}

