#include "pch.h"
#include "MenuUI.h"



#include <Engine/CPathMgr.h>
#include <Engine/CTaskMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "CLevelSaveLoad.h"
#include "AnimationEditorUI.h"
#include "TileMapEditorUI.h"



MenuUI::MenuUI()
    : UI("Menu", "##Menu")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        render_update();

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{
    File();

    Edit();

    Level();

    GameObject();

    Asset();
}

void MenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Save Level", ""))
        {
            wchar_t szSelect[256] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetSaveFileName(&ofn))
            {
                CLevelSaveLoad::SaveLevel(CLevelMgr::GetInst()->GetCurrentLevel(), CPathMgr::GetRelativePath(szSelect));
            }
        }

        if (ImGui::MenuItem("Load Level", ""))
        {
            wchar_t szSelect[256] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {
                CLevel* pLevel = CLevelSaveLoad::LoadLevel(CPathMgr::GetRelativePath(szSelect));
                CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);

                // Inspector 의 타겟정보를 nullptr 로 되돌리기
                Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
                pInspector->SetTargetObject(nullptr);
            }
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Edit()
{
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Animation Editor"))
        {
            AnimationEditorUI* Editor = (AnimationEditorUI*)CImGuiMgr::GetInst()->FindUI("##AnimationEditor");
            Editor->Activate();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("TileMap Editor"))
        {
            TileMapEditorUI* Editor = (TileMapEditorUI*)CImGuiMgr::GetInst()->FindUI("##TileEditor");
            Editor->Activate();
        }
        ImGui::Separator();
        ImGui::EndMenu();
    }
}

void MenuUI::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

        LEVEL_STATE State = pCurLevel->GetState();

        bool PlayEnable = false;
        bool PauseEnable = false;
        bool StopEnable = false;

        if (LEVEL_STATE::STOP == State || LEVEL_STATE::PAUSE == State)
            PlayEnable = true;
        else
            PlayEnable = false;

        if (LEVEL_STATE::PLAY == State)
            PauseEnable = true;
        else
            PauseEnable = false;

        if (LEVEL_STATE::PLAY == State || LEVEL_STATE::PAUSE == State)
            StopEnable = true;
        else
            StopEnable = false;


        if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
        {
            if (LEVEL_STATE::STOP == pCurLevel->GetState())
            {
                CLevelSaveLoad::SaveLevel(pCurLevel, L"Level//temp.lv");
            }

            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PLAY);
        }

        if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
        {
            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PAUSE);
        }

        if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
        {
            CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"Level//temp.lv");
            CLevelMgr::GetInst()->ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

            // Inspector 의 타겟정보를 nullptr 로 되돌리기
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
            pInspector->SetTargetObject(nullptr);
        }

        ImGui::EndMenu();
    }
}

const char* COMPONENT_TYPE_STRING[] =
{
   "Transform",
   "MeshRender",
   "Collider2D",
   "Movement",
   "Animator2D",
   "Light2D",
   "TileMap",
   "StateMachine"
};

void MenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object", ""))
		{
			CGameObject* pNewObj = new CGameObject;
			pNewObj->SetName(L"New GameObject");
			pNewObj->AddComponent(new CTransform);
			GamePlayStatic::SpawnGameObject(pNewObj, 0);
		}
		ImGui::Separator();

		if (ImGui::BeginMenu("Component", ""))
		{
			for (size_t i = 0; i < IM_ARRAYSIZE(COMPONENT_TYPE_STRING); i++)
			{
				if (ImGui::MenuItem(COMPONENT_TYPE_STRING[i]))
				{
					Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
					if (nullptr != inspector->GetTargetObject())
					{
						UINT type = i;

						switch (type)
						{
						case (UINT)eCOMPONENT::Transform:
							if (!inspector->GetTargetObject()->Transform())
								inspector->GetTargetObject()->AddComponent(new CTransform);
							break;
						case (UINT)eCOMPONENT::MeshRender:
							if (!inspector->GetTargetObject()->MeshRender())
								inspector->GetTargetObject()->AddComponent(new CMeshRender);
							break;
						case (UINT)eCOMPONENT::Collider2D:
							if (!inspector->GetTargetObject()->Collider2D())
								inspector->GetTargetObject()->AddComponent(new CCollider2D);
							break;
						case (UINT)eCOMPONENT::Movement:
							if (!inspector->GetTargetObject()->Movement())
								inspector->GetTargetObject()->AddComponent(new CMovement);
							break;
						case (UINT)eCOMPONENT::Animator2D:
							if (!inspector->GetTargetObject()->Animator2D())
								inspector->GetTargetObject()->AddComponent(new CAnimator2D);
							break;
						case (UINT)eCOMPONENT::Light2D:
							if (!inspector->GetTargetObject()->Light2D())
								inspector->GetTargetObject()->AddComponent(new CLight2D);
							break;
						case (UINT)eCOMPONENT::TileMap:
							if (!inspector->GetTargetObject()->TileMap())
								inspector->GetTargetObject()->AddComponent(new CTileMap);
							break;
						case (UINT)eCOMPONENT::StateMachine:
							if (!inspector->GetTargetObject()->StateMachine())
								inspector->GetTargetObject()->AddComponent(new CStateMachine);
							break;
						default:
							break;
						}

                        CTaskMgr::GetInst()->IsFixObject(true);
					}
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Script", ""))
		{
			vector<wstring> vecScriptName;
			CScriptMgr::GetScriptInfo(vecScriptName);

			for (size_t i = 0; i < vecScriptName.size(); ++i)
			{
				if (ImGui::MenuItem(ToString(vecScriptName[i]).c_str()))
				{
					Inspector* inspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("##Inspector");
					if (nullptr != inspector->GetTargetObject())
					{
						inspector->GetTargetObject()->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
					}
				}
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

}


void MenuUI::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Empty Material"))
        {
            wchar_t szPath[255] = {};
            wstring FilePath = CPathMgr::GetContentPath();

            int num = 0;
            while (true)
            {
                swprintf_s(szPath, L"Material\\New Material_%d.mtrl", num);
                if (!exists(FilePath + szPath))
                    break;
                ++num;
            }

            CMaterial* pMtrl = new CMaterial;
            pMtrl->SetName(szPath);
            pMtrl->Save(szPath);
            GamePlayStatic::AddAsset(pMtrl);
        }

        ImGui::EndMenu();
    }
}
