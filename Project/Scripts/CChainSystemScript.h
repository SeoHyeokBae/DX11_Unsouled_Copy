#pragma once
#include <Engine\CScript.h>

class CChainSystemScript
	: public CScript
{
private:
	bool			m_bStart;		// 시스템 시작
	bool            m_bChain;		// 체인 발동
	bool			m_bRecovery;
	bool            m_bYellow;
	
	float           m_fBlinkTime;
	float			m_fTiming;

public:
	bool IsStart() { return m_bStart; }
	bool IsChain() { return m_bChain; }
	bool IsRecovery() { return m_bRecovery; }
	
	void SetStart(bool _start) { m_bStart = _start;}
	void SetChain(bool _chain) { m_bChain = _chain; }
	
	void Clear();
public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CChainSystemScript);
public:
	CChainSystemScript();
	~CChainSystemScript();
};

