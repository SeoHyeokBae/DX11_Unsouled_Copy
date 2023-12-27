#pragma once
#include "CComponent.h"

class CMesh;
class CMaterial;

class CRenderComponent :
    public CComponent
{
private:
    CMesh*               m_Mesh;
    CMaterial*           m_Mtrl;

public:
    void SetMesh(CMesh* _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(CMaterial* _Mtrl) { m_Mtrl = _Mtrl; }

    CMesh* GetMesh() { return m_Mesh; }
    CMaterial* GetMaterial() { return m_Mtrl; }

public:
    virtual void finaltick() {};
    virtual void UpdateData() = 0;
    virtual void render() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    ~CRenderComponent();
};

