#pragma once
#include "CAsset.h"

// �𵨸� ������ ����
// ���ؽ� ����
// �ε��� ����

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

