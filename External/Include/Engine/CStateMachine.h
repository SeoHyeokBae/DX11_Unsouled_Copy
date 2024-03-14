#pragma once
#include "CComponent.h"

#include "CFSM.h"
#include "CBlackboard.h"

class CStateMachine :
    public CComponent
{
private:
    Ptr<CFSM>       m_FSM;
    CBlackboard     m_Blackboard;

public:
    void SetFSM(Ptr<CFSM> _FSM);
    void AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData);



public:
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CStateMachine);
    CStateMachine();

    ~CStateMachine();
};

