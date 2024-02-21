#pragma once
#include "CScript.h"
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

