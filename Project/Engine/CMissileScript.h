#pragma once
#include "CScript.h"
class CMissileScript :
    public CScript
{
private:
    float   m_fSpeed;

public:
    virtual void tick() override;

public:
    CMissileScript();
    ~CMissileScript();
};

