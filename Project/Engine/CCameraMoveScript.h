#pragma once
#include "CScript.h"
class CCameraMoveScript :
    public CScript
{
private:
    float       m_CamSpeed;

public:
    virtual void tick();

public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

