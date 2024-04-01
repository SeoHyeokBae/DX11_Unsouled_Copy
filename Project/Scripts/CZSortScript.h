#pragma once
#include <Engine\CScript.h>
class CZSortScript :
    public CScript
{
private:

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CZSortScript);
public:
	CZSortScript();
	~CZSortScript();
};

