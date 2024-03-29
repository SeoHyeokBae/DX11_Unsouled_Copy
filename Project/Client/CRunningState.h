#pragma once
#include <Engine\CState.h>
class CRunningState :
    public CState
{
private:
    bool m_bUP;
    bool m_bRight;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CRunningState();
    ~CRunningState();
};
