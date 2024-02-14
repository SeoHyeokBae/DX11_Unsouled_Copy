#include "pch.h"
#include "Outliner.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "TreeUI.h"


Outliner::Outliner()
	: UI("Outliner","##Outliner")
{
    m_Tree = new TreeUI("OutlinerTree");
    m_Tree->ShowRootNode(false);
    m_Tree->UseDragDrop(true);

    AddChildUI(m_Tree);

    // 트리에 클릭 이벤트 등록
    m_Tree->AddSelectDelegate(this, (Delegate_1)&Outliner::SelectObject);
    m_Tree->AddDragDropDelegate(this, (Delegate_2)&Outliner::DragDropObject);

    // 트리 내용을 현재 레벨의 물체들로 구성
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

void Outliner::SelectObject(DWORD_PTR _Node)
{
    TreeNode* pNode = (TreeNode*)_Node;
    CGameObject* pObject = (CGameObject*)pNode->GetData();

    if (nullptr == pObject)
        return;

    Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
    pInspector->SetTargetObject(pObject);
}
void Outliner::DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source)
{
    TreeNode* pDestNode = (TreeNode*)_Dest;
    TreeNode* pSourceNode = (TreeNode*)_Source;

    if (nullptr == pDestNode)
    {
        CGameObject* pSourceObj = (CGameObject*)pSourceNode->GetData();

        int LayerIdx = pSourceObj->DisconnectWithParent();

        // 원래 부모가 없었다
        if (LayerIdx == -1)
            return;

        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        pCurLevel->AddObject(pSourceObj, LayerIdx, false);
    }
    else
    {
        CGameObject* pDestObj = (CGameObject*)pDestNode->GetData();
        CGameObject* pSourceObj = (CGameObject*)pSourceNode->GetData();
        pDestObj->AddChild(pSourceObj);
    }

    ResetCurrentLevel();
}