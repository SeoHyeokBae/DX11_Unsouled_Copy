#pragma once
#include <Engine\CState.h>
class CNiug_BallistaReadyState :
    public CState
{
private:
    int     m_FrmCount;
    float   m_fAcc;

public:


public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CNiug_BallistaReadyState();
    ~CNiug_BallistaReadyState();
};

