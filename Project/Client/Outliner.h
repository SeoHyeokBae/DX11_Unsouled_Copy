#pragma once
#include "UI.h"

// 현재 레벨에 있는 모든 오브젝트들 나열


class Outliner :
    public UI
{
public:
    virtual void render_update() override;

public:
    Outliner();
    ~Outliner();
};

