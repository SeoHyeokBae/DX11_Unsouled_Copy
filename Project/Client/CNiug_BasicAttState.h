#pragma once
#include <Engine\CState.h>
class CNiug_BasicAttState :
    public CState
{
private:
    int     m_Combo;

public:
    void AddComboCount()
    {
        m_Combo++;
        if (4 < m_Combo)
        {
            m_Combo = 1;
        }
    }

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CNiug_BasicAttState();
    ~CNiug_BasicAttState();
};

