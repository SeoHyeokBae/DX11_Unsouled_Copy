#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CTransform.h"

CMeshRender::CMeshRender()
	:CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::UpdateData()
{
	if (nullptr != GetShader())
	{
		GetShader()->UpdateData();	//레이아웃, 토폴로지, 쉐이더 세팅
	}

	GetOwner()->Transform()->UpdateData();	// 상수버퍼 세팅
}

void CMeshRender::render()
{
	if (nullptr == GetMesh() || nullptr == GetShader())
		return;

	UpdateData();

	GetMesh()->render();
}