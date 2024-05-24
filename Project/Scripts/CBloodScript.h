#pragma once

#include <Engine/CScript.h>

class CGameObject;


class CBloodScript :
	public CScript
{
private:
	CMovement* m_Movement;

	Vec2 vRandDir;

	float fLife;
	float fMaxLife;
	float fFlight;
	bool bGround;

public:
	void Update_Gravity();

	void Dead();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CBloodScript);
public:
	CBloodScript();
	~CBloodScript();
};

