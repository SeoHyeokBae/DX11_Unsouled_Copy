#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	m_matWorld = XMMatrixIdentity();	// ���� ��ķ� �ʱ�ȭ 

	// XMMatrix...(..) ��� ���Ҹ� �־� �ִ� ���� �Լ���
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;


}	

void CTransform::UpdateData()
{
	tTransform transform = {};
	transform.matWorld = m_matWorld;

	// ��ġ������ Transform ������ۿ� ������, B0 �������Ϳ� ���ε� �ص�
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}