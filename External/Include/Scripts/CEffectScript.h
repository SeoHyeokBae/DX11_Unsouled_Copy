#pragma once
#include <Engine\CScript.h>

class CGameObject;
class CScriptMgr;

class CEffectScript :
    public CScript
{
private:
    CGameObject*    m_EffectObj;
    CScriptMgr*     m_ScrMgr;

public:
    CGameObject* GetEffect() { return m_EffectObj; }
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

