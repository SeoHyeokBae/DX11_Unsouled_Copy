#pragma once
#include "CScript.h"
class CBackgroundScript :
    public CScript
{
public:
    virtual void tick() override;

public:
    CBackgroundScript();
    ~CBackgroundScript();
};

