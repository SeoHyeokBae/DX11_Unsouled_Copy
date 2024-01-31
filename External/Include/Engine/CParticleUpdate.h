#pragma once
#include "CComputeShader.h"
class CParticleUpdate :
    public CComputeShader
{
private:
    
public:
    
public:
    virtual int UpdateData() override;
    virtual void Clear() override;

public: 
    CParticleUpdate();
    ~CParticleUpdate();
};

