#pragma once
#include <Engine\CScript.h>

class CGameObject;

class CHitColliderScript :
    public CScript
{
private:
    CGameObject*    m_Owner;
    float           m_fDuration;    // 기본지속 1.f;
    float           m_fAcc;
    bool            m_bOn;

public:
    CGameObject* GetSelf() { return m_Owner; }
    float GetDuration() { return m_fDuration; }

    void SetSelf(CGameObject* _owner) { m_Owner = _owner; }
    void SetDuration(float _time) { m_fDuration = _time; }

    bool IsOn() { return m_bOn; }
    void On() { m_bOn = true; }
    void Off() { m_bOn = false; }

    void Dead();
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CHitColliderScript);

public:
    CHitColliderScript();
    ~CHitColliderScript();
};

