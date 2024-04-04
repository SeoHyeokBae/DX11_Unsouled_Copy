#pragma once
#include "AssetUI.h"

class CGameObject;

class PrefabUI :
    public AssetUI
{
private:
    CGameObject* m_ViewObj;

public:
    virtual void render_update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

