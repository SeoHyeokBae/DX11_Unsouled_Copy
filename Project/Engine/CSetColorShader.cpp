#include "pch.h"
#include "CSetColorShader.h"

CSetColorShader::CSetColorShader()
	: CComputeShader(32, 32, 1)
{
}

CSetColorShader::~CSetColorShader()
{
}

int CSetColorShader::UpdateData() 
{
	if (nullptr == m_TargetTex)
		return E_FAIL;

	// ��� ����
	m_Const.iArr[0] = m_TargetTex->GetWidth();
	m_Const.iArr[1] = m_TargetTex->GetHeight();
	m_Const.v4Arr[0] = m_Color;

	// �ؽ��� U0 �� ���ε�
	if (FAILED(m_TargetTex->UpdateData_CS_UAV(0)))
	{
		return E_FAIL;
	}

	// ������ �׷� �� üũ
	UINT width = m_TargetTex->GetWidth();
	UINT height = m_TargetTex->GetHeight();

	SetGroupX(1 + width / m_ThreadX);
	SetGroupY(1 + height / m_ThreadY);
	SetGroupZ(1);

	return S_OK;

}

void CSetColorShader::Clear()
{
	m_TargetTex->Clear_CS_UAV();
}