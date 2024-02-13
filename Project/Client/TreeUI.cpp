#include "pch.h"
#include "TreeUI.h"

TreeNode::TreeNode()
	: m_bFrame(false)
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);

}

void TreeNode::render_update()
{
	string strID = m_Name + m_ID;

	UINT Flag = 0;

	if (m_bFrame)
		Flag |= ImGuiTreeNodeFlags_Framed;
	if (m_vecChildNode.empty())
		Flag |= ImGuiTreeNodeFlags_Leaf;
	if (m_bSelected)
		Flag |= ImGuiTreeNodeFlags_Selected;

	if (m_bFrame && m_vecChildNode.empty())
		strID = "   " + strID;

	if (ImGui::TreeNodeEx(strID.c_str(), Flag))
	{
		if (ImGui::IsItemClicked())
		{
			m_Owner->SetSelectedNode(this);
		}
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

	// Delegate 호출
	if (m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
		{
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
		}
	}



	m_bSelectEvent = false;
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

	pNewNode->m_Owner = this;

	return pNewNode;
}
void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	if (m_Selected)
	{
		m_Selected->m_bSelected = false;
	}

	m_Selected = _SelectedNode;

	if (nullptr != m_Selected)
	{
		m_Selected->m_bSelected = true;
	}

	m_bSelectEvent = true;
}