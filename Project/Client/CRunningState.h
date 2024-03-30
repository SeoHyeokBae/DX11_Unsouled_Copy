#pragma once
#include <Engine\CState.h>

class CAnimator2D;

class CRunningState :
    public CState
{
private:
    CAnimator2D* m_Anim;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CRunningState();
    ~CRunningState();
};
