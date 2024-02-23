#pragma once
#include <Engine/CScript.h>

class CBackgroundScript :
    public CScript
{
public:
    virtual void tick() override;

    CLONE(CBackgroundScript);
public:
    CBackgroundScript();
    ~CBackgroundScript();
};

