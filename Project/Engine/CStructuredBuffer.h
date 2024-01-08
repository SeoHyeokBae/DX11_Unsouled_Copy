#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};

class CStructuredBuffer :
    public CEntity
{

private:
    ComPtr<ID3D11Buffer>                m_SB;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;

    UINT                                m_ElementSize;
    UINT                                m_ElementCount;

    SB_TYPE                             m_Type;

public:
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE = SB_TYPE::READ_ONLY, void* _pSysMem = nullptr);
    void UpdateData(UINT _RegisterNum);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

