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

TreeUI::TreeUI(const string& _ID)
	: UI("", _ID)
{
	m_Root = new TreeNode;
	m_Root->m_Name = "Root";
	m_Root->m_ID = "##1";

	TreeNode* pChild = new TreeNode;
	pChild->m_Name = "Child";
	pChild->m_ID = "##2";

	m_Root->m_vecChildNode.push_back(pChild);

}

TreeUI::~TreeUI()
{
	delete m_Root;
}

void TreeUI::render_update()
{
	if (nullptr != m_Root)
	{
		m_Root->render_update();
	}
}
