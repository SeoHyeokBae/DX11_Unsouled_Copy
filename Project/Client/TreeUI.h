#pragma once
#include "UI.h"

class TreeNode
{
public:
    string              m_Name;
    string              m_ID;
    vector<TreeNode*>   m_vecChildNode;

    DWORD_PTR           m_Data;

public:
    void render_update();

public:
    TreeNode();
    ~TreeNode();
};


class TreeUI :
    public UI
{
private:
    TreeNode*   m_Root;

public:
    virtual void render_update() override;

public:
    TreeUI(const string& _ID);
    ~TreeUI();
};

