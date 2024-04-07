#pragma once
#include <Engine\CState.h>
class CNiug_BallistaSoulderState :
    public CState
{
private:
    int     m_FrmCount;

public:


public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CNiug_BallistaSoulderState();
    ~CNiug_BallistaSoulderState();
};

