#pragma once
#include <Engine\CState.h>

class CAnimator2D;
class CChainSystemScript;
class CPlayerAttColScript;

class CAttackState :
    public CState
{
private:
    CAnimator2D*             m_Anim;
    CChainSystemScript*      m_ChainSystem;
    CPlayerAttColScript*     m_AttCol;
    int                      m_Combo;

public:
    int GetCombo() { return m_Combo; }

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CAttackState();
    ~CAttackState();
};

