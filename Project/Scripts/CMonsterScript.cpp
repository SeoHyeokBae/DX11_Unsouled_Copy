#include "pch.h"
#include "CMonsterScript.h"

CMonsterScript::CMonsterScript()
	: CScript(MONSTERSCRIPT)
	, m_DetectRange(400.f)
	, m_Speed(200.f)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"DetectRange", BB_DATA::FLOAT, &m_DetectRange);
		StateMachine()->AddBlackboardData(L"Speed", BB_DATA::FLOAT, &m_Speed);

		// �÷��̾ ã�Ƽ� Object Ÿ������ �����忡 ����Ѵ�.
		//StateMachine()->AddBlackboardData(L"TargetObject", BB_DATA::OBJECT, );
	}
}

void CMonsterScript::tick()
{
}

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

