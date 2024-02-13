#pragma once
#include "UI.h"

// 현재 레벨에 있는 모든 오브젝트들 나열

class TreeUI;
class TreeNode;

class Outliner :
    public UI
{
private:
    TreeUI*             m_Tree;

    vector<string>      m_vecStr;


public:
    virtual void render_update() override;

public:
    void ResetCurrentLevel();

private:
    void AddObjectToTree(TreeNode* _Node, CGameObject* _Object);

public:
    Outliner();
    ~Outliner();
};

