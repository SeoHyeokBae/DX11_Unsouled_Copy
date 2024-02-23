#include "pch.h"
#include "CBackgroundScript.h"

#include <Engine/CKeyMgr.h>

CBackgroundScript::CBackgroundScript()
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
		i ? MeshRender()->GetMaterial()->SetScalarParam(INT_0, 1) : MeshRender()->GetMaterial()->SetScalarParam(INT_0, 0);
		i = !i;
	}
}
