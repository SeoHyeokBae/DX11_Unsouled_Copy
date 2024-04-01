#pragma once
#include <Engine\CState.h>
class CDashState :
    public CState
{
private:

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CDashState();
    ~CDashState();
};

