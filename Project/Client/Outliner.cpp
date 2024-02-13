#include "pch.h"
#include "Outliner.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include "TreeUI.h"

Outliner::Outliner()
	: UI("Outliner","##Outliner")
{
    m_Tree = new TreeUI("OutlinerTree");
    m_Tree->ShowRootNode(false);
    AddChildUI(m_Tree);

    ResetCurrentLevel();
}

Outliner::~Outliner()
{
}

void Outliner::render_update()
{
	ImGui::SeparatorText("Current Level All Object");

}

void Outliner::ResetCurrentLevel()
{
    // Ʈ�� ������ ����
    m_Tree->ClearNode();

    // Ʈ���� ��Ʈ �߰�
    TreeNode* pRoodNode = m_Tree->AddTreeNode(nullptr, "DummyRoot", 0);

    // ���� ������ �����´�
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        CLayer* pLayer = pCurLevel->GetLayer(i);
        const vector<CGameObject*>& vecParent = pLayer->GetParentObjects();

        for (size_t i = 0; i < vecParent.size(); i++)
        {
            AddObjectToTree(pRoodNode, vecParent[i]);
        }
    }
}

void Outliner::AddObjectToTree(TreeNode* _Node, CGameObject* _Object)
{
    TreeNode* pNode = m_Tree->AddTreeNode(_Node, string(_Object->GetName().begin(), _Object->GetName().end()), (DWORD_PTR)_Object);

    const vector<CGameObject*>& vecChild = _Object->GetChild();

    for (size_t i = 0; i < vecChild.size(); i++)
    {
        AddObjectToTree(pNode, vecChild[i]);
    }
}
