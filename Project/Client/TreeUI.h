#pragma once
#include "UI.h"

class TreeNode
{
public:
    string              m_Name;
    string              m_ID;
    vector<TreeNode*>   m_vecChildNode;

    TreeNode*           m_ParentNode;
    DWORD_PTR           m_Data;

public:
    void SetName(string& _Name) { m_Name = _Name; }

private:
    void SetID(const string& _ID) { m_ID = _ID; }
    void AddChildeNode(TreeNode* _Node)
    {
        m_vecChildNode.push_back(_Node);
        _Node->m_ParentNode = this;
    }

public:
    void render_update();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};


class TreeUI :
    public UI
{
private:
    static UINT NodeID;

private:
    TreeNode*   m_Root;
    bool        m_bShowRoot;

public:
    virtual void render_update() override;

public:
    void ShowRootNode(bool _bShow) { m_bShowRoot = _bShow; }
    TreeNode* AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData);
    void ClearNode()
    {
        if (nullptr != m_Root)
        {
            delete m_Root;
            m_Root = nullptr;
        }
    }

public:
    TreeUI(const string& _ID);
    ~TreeUI();
};

