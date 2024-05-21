#pragma once
#include <Engine\CScript.h>

// EFFECT STATUS
#define EFFECTMAX        3
#define CHAIN  	        0x001
#define SPARK		    0x002
#define HITCIRCLE       0x004

class CGameObject;
class CScriptMgr;
class CPrefab;

enum class eEffectStatus
{
    CHAIN_EFFECT,
    SWORD_SPARK,
    HIT_CIRCLE,
    NONE,
};

class CEffectScript : 
    public CScript
{
private:
    Ptr<CPrefab>            m_EffectPrefab;
    CScriptMgr*             m_ScrMgr;

    UINT                    m_iStatus;
    
    Vec2                    m_vCalculatedPos;
    Vec3                    m_vCalculatedRot;

    map<int, CGameObject*>  m_RegisterObj;

public:
    void OnEffect(eEffectStatus _status);
    void OffEffect() { m_iStatus &= 0; };

    void SetCalculatedPos(Vec2 _pos) { m_vCalculatedPos = _pos; }
    void SetCalculatedRot(Vec3 _rot) { m_vCalculatedRot = _rot; }

    void Dead(CGameObject* _obj, int _effNum);

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

