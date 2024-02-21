#pragma once
#include "CEntity.h"
class CConstBuffer :
    public CEntity
{
private:
    D3D11_BUFFER_DESC       m_Desc;
    ComPtr<ID3D11Buffer>    m_CB;

    UINT                    m_ElementSize;
    UINT                    m_ElementCount; // 요소(상수버퍼) 개수

    CB_TYPE                 m_Type;

public:
    int Create(UINT _ElementSize, UINT _ElementCount);
    void UpdateData();
    void SetData(void* _Src, UINT _ElementCount = 0);
    void UpdateData_CS();
    
    CLONE_DISABLE(CConstBuffer);

public:
    CConstBuffer(CB_TYPE _Type);
    ~CConstBuffer();
};

