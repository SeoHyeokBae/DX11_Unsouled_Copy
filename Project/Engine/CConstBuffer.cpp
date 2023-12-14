#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer()
	: m_Desc{}
{
}

CConstBuffer::~CConstBuffer()
{

}
int CConstBuffer::Create(UINT _ElementSize, UINT _ElementCount)
{
	m_ElementSize = _ElementSize;
	m_ElementCount = _ElementCount;

	// 상수 버퍼(Constant Buffer) 생성
	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.StructureByteStride = m_ElementSize;		// 요소 간격
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 버퍼에 데이터 쓰기 가능
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;

	// 상수 버퍼 생성
	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		MessageBox(nullptr, L"상수 버퍼 생성 실패", L"TestInit 오류", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CConstBuffer::SetData(void* _Src, UINT _ElementCount)
{
	if (0 == _ElementCount)	// 요소 갯수 기본 최대
	{
		_ElementCount = m_ElementCount;
	}

	//// SystemMem -> GPUMem
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	// 상수버퍼 사용 시
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);		// cpu 데이터 메모리와 gpu 데이터 메모리의 일치화
	memcpy(tSub.pData, _Src, m_ElementSize * _ElementCount);			// cpu 데이터 메모리에 수정된 데이터 복사
	CONTEXT->Unmap(m_CB.Get(), 0);										// cpu 데이터를 gpu 데이터 메모리로 보냄
}

void CConstBuffer::UpdateData(UINT _RegisterNum)
{
	// 상수버퍼 전달 (위치) (바인딩)
	CONTEXT->VSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
}

