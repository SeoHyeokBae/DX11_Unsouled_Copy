#pragma once
#include <Engine\CState.h>

class CAnimator2D;

class CAttackState :
    public CState
{
private:
    CAnimator2D*     m_Anim;

    int              m_Combo;
    float            m_fTiming;
    bool             m_bChain;
    bool             m_bStart;

public:
    bool IsChain() { return m_bChain; }

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CAttackState();
    ~CAttackState();
};

