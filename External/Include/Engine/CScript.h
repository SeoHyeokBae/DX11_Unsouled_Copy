#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
//#include "CResMgr.h"

#include "CGameObject.h"
#include "components.h"


class CScript :
    public CComponent
{
private:
    UINT    m_iScriptType;

public:
    virtual void finaltick() final {} // script는 tick에서 동작 finaltick 은 상수버퍼

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}

    CScript* Clone() = 0;
public:
    CScript();
    ~CScript();
};

