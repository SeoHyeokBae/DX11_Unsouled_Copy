#include "pch.h"
#include "CChainSystemScript.h"

#include <Engine/CTimeMgr.h>

#define CHAIN_START 0.015f
#define CHAIN_OVER 0.07f
#define BLINK_INTERVAL 0.03f

// ü�� Ÿ�̹� �ȿ� ��ų ���� ����Ʈ

CChainSystemScript::CChainSystemScript()
	: CScript(CHAINSYSTEMSCRIPT)
	, m_bStart(false)
	, m_bYellow(false)
	, m_bChain(false)
	, m_bRecovery(false)
	, m_bEffect(false)
    , m_fBlinkTime(0.f)
	, m_fTiming(0.f)
{
}
CChainSystemScript::~CChainSystemScript()
{
}

void CChainSystemScript::begin()
{
}

void CChainSystemScript::tick()
{
	if (m_bStart)
	{
		m_fTiming += DT;
	}

	// ü�� Ÿ�̹�
	if (m_fTiming >= CHAIN_START && m_fTiming <= CHAIN_OVER)
		m_bChain = true;


	if (RECOVERYTIME > m_fTiming)
	{
		m_bRecovery = true;
	}

	if (m_bRecovery && RECOVERYTIME <= m_fTiming)
	{
		m_bRecovery = false;
	}

	// ä�ιߵ��� ������
	if (m_bChain)
	{
		m_fBlinkTime += DT;
		if (m_fBlinkTime >= BLINK_INTERVAL)
		{
			if (!m_bYellow)
			{
				m_bYellow = true;
				GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 1);
			}
			else
			{
				m_bYellow = false;
				GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 0);
			}

			m_fBlinkTime = 0.f;
		}
	}
	else
	{
		m_bYellow = false;
		GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, 0);
	}

}

void CChainSystemScript::Clear()
{
	m_bStart = false;
	m_bChain = false;
	m_bRecovery = false;
	m_bEffect = false;
	m_fBlinkTime = 0.f;
	m_fTiming = 0.f;
}

void CChainSystemScript::SaveToFile(FILE* _File)
{
}

void CChainSystemScript::LoadFromFile(FILE* _File)
{
}


