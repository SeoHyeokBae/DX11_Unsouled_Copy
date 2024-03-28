#pragma once

#include <Engine/CScript.h>

class CGameObject;

class CShadowScript :
    public CScript
{
private:
	CGameObject* m_ShadowObj;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CShadowScript);
public:
	CShadowScript();
	~CShadowScript();
};

