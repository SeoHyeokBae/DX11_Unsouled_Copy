#pragma once
#include <Engine/CState.h>

class CAnimator2D;

class CAbsorbState :
    public CState
{
private:
    eDIR m_Dir;
    CAnimator2D* m_Anim;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CAbsorbState();
    ~CAbsorbState();
};

