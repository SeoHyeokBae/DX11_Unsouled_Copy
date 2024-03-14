#pragma once
#include "CAsset.h"

#include "CState.h"
#include "CBlackboard.h"

class CStateMachine;

class CFSM :
    public CAsset
{
private:
    map<wstring, CState*>   m_mapState;
    CBlackboard             m_Blackboard;

public:
    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);

public:
    void finaltick(CStateMachine* _StateMachine);

public:
    CLONE_DISABLE(CFSM);
    CFSM(bool _bEngine = false);
    ~CFSM();
};

