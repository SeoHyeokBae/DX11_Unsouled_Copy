#pragma once
#include <Engine\CState.h>
class CNiug_RunningState :
    public CState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CNiug_RunningState();
    ~CNiug_RunningState();
};

