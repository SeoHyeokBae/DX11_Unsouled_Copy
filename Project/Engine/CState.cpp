#include "pch.h"
#include "CState.h"

#include "CStateMachine.h"
#include "CGameObject.h"

CState::CState()
	: m_FSM(nullptr)
{
}

CState::~CState()
{
}

void* CState::GetBlackboardData(const wstring& _strKey)
{
	CStateMachine* pSM = m_FSM->GetStateMachine();
	return pSM->GetBlackboardData(_strKey);
}
void CState::SetBlackboardData(const wstring& _strKey, void* _pData)
{
	CStateMachine* pSM = m_FSM->GetStateMachine();
	pSM->SetBlackboardData(_strKey, _pData);
}
void CState::ChangeState(const wstring& _strStateName)
{
	assert(m_FSM);
	m_FSM->ChangeState(_strStateName);
}