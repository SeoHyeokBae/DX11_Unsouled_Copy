#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"
#include <Engine/components.h>
#include <Engine/CCameraMoveScript.h>

#include <Engine/CRenderMgr.h>


CEditorObjMgr::CEditorObjMgr()
{

}

CEditorObjMgr::~CEditorObjMgr()
{

}

void CEditorObjMgr::init()
{
	// 에디터용 카메라 오브젝트 생성
	CGameObjectEx* pEditorCam = new CGameObjectEx;
	pEditorCam->AddComponent(new CTransform);
	pEditorCam->AddComponent(new CCamera);
	pEditorCam->AddComponent(new CCameraMoveScript);

	m_vecEditorObj.push_back(pEditorCam);

	// Editor 용 카메라로서 렌더매니저에 등록
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());
}

void CEditorObjMgr::progress()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}
