#include "pch.h"
#include "CStateMachine.h"

CStateMachine::CStateMachine()
	: CComponent(COMPONENT_TYPE::STATEMACHINE)
{
}

CStateMachine::~CStateMachine()
{
}


void CStateMachine::finaltick()
{
	m_FSM->finaltick(this);
}

void CStateMachine::SetFSM(Ptr<CFSM> _FSM)
{
	m_FSM = _FSM;
}

void CStateMachine::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _pData)
{
	m_Blackboard.AddBlackboardData(_strKey, _Type, _pData);
}

void CStateMachine::SaveToFile(FILE* _File)
{
}

void CStateMachine::LoadFromFile(FILE* _File)
{
}

