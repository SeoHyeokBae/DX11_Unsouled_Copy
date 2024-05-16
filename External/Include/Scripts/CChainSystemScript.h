#pragma once
#include <Engine\CScript.h>

class CChainSystemScript
	: public CScript
{
private:
	bool			m_bStart;		// �ý��� ����
	bool            m_bChain;		// ü�� �ߵ�
	bool			m_bRecovery;
	bool            m_bYellow;
	bool			m_bEffect;		// ����Ʈ
	
	float           m_fBlinkTime;
	float			m_fTiming;

public:
	bool IsStart() { return m_bStart; }
	bool IsChain() { return m_bChain; }
	bool IsRecovery() { return m_bRecovery; }
	
	void SetStart(bool _start) { m_bStart = _start;}
	void SetChain(bool _chain) { m_bChain = _chain; }
	void EffectOn() { m_bEffect = true; }

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

