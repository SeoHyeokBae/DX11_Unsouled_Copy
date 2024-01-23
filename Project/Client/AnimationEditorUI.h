#pragma once
#include "UI.h"
class AnimationEditorUI :
    public UI
{


public:
    virtual void render_update() override;
    virtual void Deactivate() override;

public:
    AnimationEditorUI();
    ~AnimationEditorUI();
};

