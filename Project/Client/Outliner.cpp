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
    // 트리 내용을 삭제
    m_Tree->ClearNode();

    // 트리에 루트 추가
    TreeNode* pRoodNode = m_Tree->AddTreeNode(nullptr, "DummyRoot", 0);

    // 현재 레벨을 가져온다
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
