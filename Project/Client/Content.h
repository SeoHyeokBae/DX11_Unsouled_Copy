#pragma once
#include "UI.h"

// AssetMgr�� �ε��Ǿ� �ִ� ��� ���µ��� ����
class Content :
    public UI
{

public:
    virtual void render_update() override;


public:
    Content();
    ~Content();
};

