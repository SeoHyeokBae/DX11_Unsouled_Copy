#pragma once
#include <Engine\CScript.h>

// EFFECT STATUS
#define STATUS_MAX		2
#define CHAIN  	        0x001
#define SPARK		    0x002

class CGameObject;
class CScriptMgr;
class CPrefab;

enum class eEffectStatus
{
    CHAIN_EFFECT,
    SWORD_SPARK,
    NONE,
};

class CEffectScript :
    public CScript
{
private:
    Ptr<CPrefab>            m_EffectPrefab;
    CScriptMgr*             m_ScrMgr;

    UINT                    m_iStatus;

    map<int, CGameObject*>  m_RegisterObj;

public:
    void OnEffect(eEffectStatus _status);
    void OffEffect() { m_iStatus &= 0; };

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CEffectScript);
public:
    CEffectScript();
    ~CEffectScript();
};

