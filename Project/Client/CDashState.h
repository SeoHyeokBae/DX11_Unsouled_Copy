#pragma once
#include <Engine\CState.h>

class CAnimator2D;
class CChainSystemScript;

class CDashState :
    public CState
{
private:
    CAnimator2D*         m_Anim;
    CChainSystemScript*  m_ChainSystem;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CDashState();
    ~CDashState();
};
