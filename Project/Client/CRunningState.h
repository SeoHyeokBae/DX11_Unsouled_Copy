#pragma once
#include <Engine\CState.h>


class CRunningState :
    public CState
{
private:
    eDIR m_Dir;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CRunningState();
    ~CRunningState();
};
