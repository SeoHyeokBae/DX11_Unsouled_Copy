#pragma once
#include "UI.h"

// ���� ������ �ִ� ��� ������Ʈ�� ����


class Outliner :
    public UI
{
public:
    virtual void render_update() override;

public:
    Outliner();
    ~Outliner();
};

