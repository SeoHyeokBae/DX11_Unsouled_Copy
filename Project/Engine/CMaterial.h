#pragma once
#include "CAsset.h"

class CGraphicsShader;
class CTexture;

class CMaterial :
    public CAsset
{
public:
    tMtrlConst          m_Const;
    CTexture*           m_arrTex[(UINT)TEX_PARAM::END];

    CGraphicsShader*    m_pShader;

public:
    void SetShader(CGraphicsShader* _Shader) { m_pShader = _Shader; }
    CGraphicsShader* GetShader() { return m_pShader; }

    void UpdateData();

public:
    CMaterial();
    ~CMaterial();

};

