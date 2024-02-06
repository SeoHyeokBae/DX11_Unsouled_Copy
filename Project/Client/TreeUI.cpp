#include "pch.h"
#include "TreeUI.h"

TreeNode::TreeNode()
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);

}

void TreeNode::render_update()
{
	string strID = m_Name + m_ID;

	if (ImGui::TreeNode(strID.c_str()))
	{
		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
}

UINT TreeUI::NodeID = 0;

TreeUI::TreeUI(const string& _ID)
	: UI("", _ID)
	, m_bShowRoot(true)
{
}

TreeUI::~TreeUI()
{
	delete m_Root;
}

void TreeUI::render_update()
{
	if (nullptr == m_Root)
		return;

	if (m_bShowRoot)
	{
		m_Root->render_update();
	}

	else
	{
		for (size_t i = 0; i < m_Root->m_vecChildNode.size(); i++)
		{
			m_Root->m_vecChildNode[i]->render_update();
		}
	}
}

TreeNode* TreeUI::AddTreeNode(TreeNode* _Parent, string _strName, DWORD_PTR _dwData)
{
	TreeNode* pNewNode = new TreeNode;
	pNewNode->m_Data = _dwData;
	pNewNode->SetName(_strName);

	// 노드마다 겹치지 않는 숫자를 ## 뒤에 ID로 붙인다
	UINT id = NodeID++;

	char buff[50] = {};
	sprintf_s(buff, "##%d", id);
	pNewNode->SetID(buff);

	if (nullptr == _Parent)
	{
		assert(!m_Root);

		m_Root = pNewNode;
	}
	else
	{
		_Parent->AddChildeNode(pNewNode);
	}

	return pNewNode;
}
