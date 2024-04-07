#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CBossNiugAttColScript :
    public CScript
{
private:
    bool m_bOn;

public:
    bool IsOn() { return m_bOn; }
    void On() { m_bOn = true; }
    void Off() { m_bOn = false; }
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CBossNiugAttColScript);

public:
    CBossNiugAttColScript();
    ~CBossNiugAttColScript();
};

