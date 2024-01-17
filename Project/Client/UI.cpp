#include "pch.h"
#include "UI.h"

UI::UI(const string& _strName, const string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bActive(true)
	, m_Parent(nullptr)
{
}

UI::~UI()
{
	Delete_Vec(m_vecChildUI);
}


void UI::tick()
{
}

void UI::render()
{
	if (!m_bActive)
		return;

	if (nullptr == GetParentUI())	// UI�� �ֻ��� �Ͻ� Begin() ~ End()
	{
		ImGui::Begin(string(m_strName + m_strID).c_str(), &m_bActive);

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::End();
	}

	else // ���� �ܰ� UI �Ͻ� BeginChild() ~ EndChild()
	{
		ImGui::BeginChild(string(m_strName + m_strID).c_str());

		render_update();	// ���� ä���

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}