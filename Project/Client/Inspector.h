#pragma once
#include "UI.h"

// GameObject ����
//  - ������Ʈ
//  - ��ũ��Ʈ

// Asset ����
//  - �� ������ ����

#include <Engine/CGameObject.h>
#include <Engine/CAsset.h>

class Inspector :
    public UI
{
private:
    CGameObject*    m_TargetObject;
    Ptr<CAsset>     m_TargetAsset;


public:
    virtual void tick() override;
    virtual void render_update() override;


public:
    void SetTargetObject(CGameObject* _Object);
    void SetTargetAsset(Ptr<CAsset> _Asset);

public:
    Inspector();
    ~Inspector();
};

