#pragma once
#include "CEntity.h"

class CFSM;
class CStateMachine;

class CState :
    public CEntity
{
private:
    CFSM*           m_FSM;

protected:
    eDIR            m_Dir;

public:
    virtual void finaltick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

protected:
    void* GetBlackboardData(const wstring& _strKey);
    void SetBlackboardData(const wstring& _strKey, void* _pData);
    CFSM* GetFSM() { return m_FSM; }
    void SetFSM(CFSM* _fsm) { m_FSM = _fsm; }

    void ChangeState(const wstring& _strStateName);

public:
    CLONE_DISABLE(CState);

    CState();
    ~CState();

    friend class CFSM;
};
