#include "pch.h"
#include "CMaterial.h"

#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CMaterial::CMaterial()
	: CAsset(ASSET_TYPE::MATERIAL)
	, m_Const{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData();

	if (nullptr != m_pShader)
	{
		m_pShader->UpdateData();
	}
}