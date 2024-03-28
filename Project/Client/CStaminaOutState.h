#pragma once
#include <Engine\CState.h>
class CStaminaOutState :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CStaminaOutState();
    ~CStaminaOutState();
};

