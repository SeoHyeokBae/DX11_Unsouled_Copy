#pragma once
#include <Engine\CState.h>

class  CAnimator2D;
class CChainSystemScript;
class CPlayerAttColScript;

class CDashAttState :
    public CState
{
private:
    CAnimator2D*            m_Anim;
    CChainSystemScript*     m_ChainSystem;
    CPlayerAttColScript*    m_AttCol;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CDashAttState();
    ~CDashAttState();
};

