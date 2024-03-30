#pragma once
#include <Engine\CState.h>

class CAnimator2D;

class CAttackState :
    public CState
{
private:
    CAnimator2D*     m_Anim;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CAttackState();
    ~CAttackState();
};

