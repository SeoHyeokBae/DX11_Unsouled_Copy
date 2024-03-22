#pragma once
#include "CAsset.h"

// 모델링 정보를 저장
// 버텍스 버퍼
// 인덱스 버퍼

class CMesh :
    public CAsset
{ 
private:
    ComPtr<ID3D11Buffer>	m_VB = nullptr;
    ComPtr<ID3D11Buffer>	m_IB = nullptr;

    D3D11_BUFFER_DESC       m_VBDesc;
    D3D11_BUFFER_DESC       m_IBDesc;

    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    void*                   m_VtxSysMem;
    void*                   m_IdxSysMem;

private:
    void UpdateData();


public:
    int Create(void* _Vts, UINT _VtxCount, void* _Idx, UINT _IdxCount);
    void render();
    void render_asparticle(UINT ParticleCount);

    CLONE_DISABLE(CMesh);
public:
    CMesh(bool _Engine = false);
    ~CMesh();
};

