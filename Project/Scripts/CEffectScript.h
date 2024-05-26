#pragma once
#include <Engine\CScript.h>

// EFFECT STATUS
#define EFFECTMAX       4
#define CHAIN  	        0x001
#define SWORDSPARK	    0x002
#define HITCIRCLE       0x004
#define BLOOD           0x008

class CGameObject;
class CScriptMgr;
class CPrefab;

enum class eEffectStatus
{
    CHAIN_EFFECT,
    SWORD_SPARK_EFFECT,
    HITCIRCLE_EFFECT,
    BLOOD_EFFECT,
    NONE,
};

enum class eBloodDir
{
    RIGHTDOWN,
    RIGHTUP,
    RIGHT,
    LEFTDOWN = 10,
    LEFTUP,
    LEFT,
    NONE,
};

class CEffectScript : 
    public CScript
{
private:
    Ptr<CPrefab>            m_EffectPrefab;
    Ptr<CPrefab>            m_BloodPrefab;
    CScriptMgr*             m_ScrMgr;
    eBloodDir               m_eDir;
    UINT                    m_iStatus;
    
    Vec2                    m_vCalculatedPos;
    Vec3                    m_vCalculatedRot;

    map<int, CGameObject*>  m_RegisterObj;

    bool                    m_bRed;
    float                   m_bRedTime;

public:
    void OnEffect(eEffectStatus _status);
    void OffEffect() { m_iStatus &= 0; };
    void OnRed() { m_bRed = true; }

    void SetCalculatedPos(Vec2 _pos) { m_vCalculatedPos = _pos; }
    void SetCalculatedRot(Vec3 _rot) { m_vCalculatedRot = _rot; }

    void SetBloodDir(eBloodDir _dir) { m_eDir = _dir; }

    void RandPlayBloodEff(CGameObject* _BloodObj);
    void CreateBlood();

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

