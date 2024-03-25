#include "pch.h"
#include "CCameraPlayScript.h"
#include <Engine/CLevelMgr.h>

#include "../Client/TileMapEditorUI.h"

CGameObject* CCameraPlayScript::m_Target = nullptr;

CCameraPlayScript::CCameraPlayScript()
	: CScript(CAMERAPLAYSCRIPT)
	, m_CamSpeed(500.f)
{
}

CCameraPlayScript::~CCameraPlayScript()
{
}

void CCameraPlayScript::tick()
{
	if (nullptr != m_Target)
	{
		Vec3 vPos = m_Target->Transform()->GetRelativePos();
		Transform()->SetRelativePos(vPos);
	}


	if (KEY_TAP(KEY::P))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
		{
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}


	if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	{
		MoveOrthographic();
	}

	else
	{
		MovePerspective();
	}

	if (KEY_PRESSED(KEY::_1))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() + DT_ENGINE * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() + DT_ENGINE * 2.f);
	}

	if (KEY_PRESSED(KEY::_2))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() - DT_ENGINE * 0.2f);
		else
			Camera()->SetFOV(Camera()->GetFOV() - DT_ENGINE * 2.f);
	}
}

void CCameraPlayScript::MoveOrthographic()
{
	TileMapEditorUI* Editor = (TileMapEditorUI*)CImGuiMgr::GetInst()->FindUI("##TileEditor");
	if (Editor->IsActivate())
		return;


	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT_ENGINE * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::B))
	{
		vRot.x += DT_ENGINE * XM_PI;
	}
	if (KEY_PRESSED(KEY::V))
	{
		vRot.x -= DT_ENGINE * XM_PI;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
}

void CCameraPlayScript::MovePerspective()
{

	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(KEY::W))
	{
		vPos += DT_ENGINE * m_CamSpeed * vFront;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos += DT_ENGINE * m_CamSpeed * -vFront;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos += DT_ENGINE * m_CamSpeed * -vRight;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += DT_ENGINE * m_CamSpeed * vRight;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT_ENGINE * XM_PI * 4.f;
		vRot.x += vDrag.y * DT_ENGINE * XM_PI * 4.f;
		Transform()->SetRelativeRotation(vRot);
	}
}

void CCameraPlayScript::SaveToFile(FILE* _File)
{

}

void CCameraPlayScript::LoadFromFile(FILE* _File)
{

}