#include "pch.h"
#include "CCameraMoveScript.h"

CCameraMoveScript::CCameraMoveScript()
	:m_CamSpeed(500.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}


void CCameraMoveScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * m_CamSpeed;
	}



	Transform()->SetRelativePos(vPos);
}