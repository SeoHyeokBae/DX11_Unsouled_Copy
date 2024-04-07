#pragma once
#include <Engine\CState.h>

class CPrefab;
class CGameObject;

class CNiug_RageOfIsno :
    public CState
{
private:
    int             m_FrmCount;
    int             m_LeftOrRight;
    eDIR            m_PrvDir;
    bool            m_bOn;
    


public:

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CNiug_RageOfIsno();
    ~CNiug_RageOfIsno();
};

