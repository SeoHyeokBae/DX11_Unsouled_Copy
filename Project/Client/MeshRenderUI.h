#pragma once
#include "ComponentUI.h"
class MeshRenderUI :
    public ComponentUI
{

public:
    virtual void render_update() override;

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

