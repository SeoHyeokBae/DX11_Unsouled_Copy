#include "pch.h"
#include "Outliner.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include "CImGuiMgr.h"
#include "Inspector.h"


Outliner::Outliner()
	: UI("Outliner","##Outliner")
{
    m_CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    m_CurLevel->GetObjectName(m_vecStr);
}

Outliner::~Outliner()
{
}

void Outliner::render_update()
{
	ImGui::SeparatorText("Current Level All Object");
	static int item_current_idx = -1;
	for (int i = 0; i < m_vecStr.size(); i++)
	{
		const bool is_selected = (item_current_idx == i);

		if (ImGui::Selectable(m_vecStr[i].c_str(), is_selected))
			item_current_idx = i;

        // 리스트 항목 중 더블클릭이 발생한다면            
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            m_strDBClicked = m_vecStr[i];

            ObjectSelect(m_strDBClicked);
        }
	}

}

void Outliner::ObjectSelect(const string& _ptr)
{
    string strObj = _ptr;
    wstring strObjName = ToWString(strObj);
    CGameObject* pObject = m_CurLevel->FindObjectByName(strObjName);
    Inspector* inspec = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
    inspec->SetTargetObject(pObject);
}