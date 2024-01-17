#include "pch.h"
#include "UI.h"

UI::UI(const string& _strName, const string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bActive(true)
{
}

UI::~UI()
{
}


void UI::tick()
{
}

void UI::render()
{
	if (m_bActive)
	{
		ImGui::Begin(string(m_strName + m_strID).c_str(), &m_bActive);

		render_update();

		ImGui::End();
	}
}