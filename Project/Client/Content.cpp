#include "pch.h"
#include "Content.h"

#include <Engine/CAssetMgr.h>

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "TreeUI.h"

Content::Content()
	: UI("Content","##Content")
{
	// ContentUI 자식으로 Tree를 지정
	m_Tree = new TreeUI("ContentTree");
	m_Tree->ShowRootNode(false);
	AddChildUI(m_Tree);

	// AssetMgr 의 에셋상태를 트리에 적용한다.
	ResetContent();

	// 트리에 Delegate 를 등록한다.
	m_Tree->AddSelectDelegate(this, (Delegate_1)&Content::SelectAsset);
}

Content::~Content()
{
}

void Content::render_update()
{
}


void Content::ResetContent()
{
	m_Tree->ClearNode();
	TreeNode* RootNode = m_Tree->AddTreeNode(nullptr, "Root", 0);

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* CategoryNode = m_Tree->AddTreeNode(RootNode, ASSET_TYPE_STRING[i], 0);

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			m_Tree->AddTreeNode(CategoryNode
				, string(pair.first.begin(), pair.first.end())
				, (DWORD_PTR)pair.second.Get());
		}
	}
}

void Content::SelectAsset(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	if (nullptr == pNode)
		return;

	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
	if (nullptr == pAsset)
		return;

	// 선택한 에셋을 Inspector 에게 알려준다.
	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetAsset(pAsset);
}
