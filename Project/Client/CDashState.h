#pragma once
#include <Engine\CState.h>

class CAnimator2D;
class CChainSystemScript;
class CMovement;
class CEffectScript;

class CDashState :
    public CState
{
private:
    CAnimator2D*         m_Anim;
    CMovement*           m_MoveMent;
    CChainSystemScript*  m_ChainSystem;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CDashState();
    ~CDashState();
};

