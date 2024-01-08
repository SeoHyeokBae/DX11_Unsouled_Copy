#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_ElementSize(0)
    , m_ElementCount(0)
    , m_Type(SB_TYPE::READ_ONLY)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}
int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE, void* _pSysMem)
{
    // 구조화버퍼 크기가 16의 배수인지 체크
    assert(!(_ElementSize % 16));

    m_ElementSize = _ElementSize;
    m_ElementCount = _ElementCount;

    D3D11_BUFFER_DESC tDesc = {};
    tDesc.ByteWidth = m_ElementSize * m_ElementCount;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;   // texture와 같음
    tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    tDesc.StructureByteStride = m_ElementSize;

    // DX 구조상 읽기/쓰기 가 동시에 이뤄지지 못함
    tDesc.CPUAccessFlags = 0;
    tDesc.Usage = D3D11_USAGE_DEFAULT;

    HRESULT hr = E_FAIL;
    if (nullptr == _pSysMem)
    {
        hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB.GetAddressOf());
    }
    else
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = _pSysMem;
        hr = DEVICE->CreateBuffer(&tDesc, &tSub, m_SB.GetAddressOf());
    }

    if (FAILED(hr)) return E_FAIL;

    // Shader Resource View 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    SRVDesc.Buffer.NumElements = m_ElementCount;

    hr = DEVICE->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf());
    if (FAILED(hr)) return E_FAIL;

    return S_OK;
}

void CStructuredBuffer::UpdateData(UINT _RegisterNum)
{
    CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

