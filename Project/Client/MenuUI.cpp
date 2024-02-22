#include "pch.h"
#include "MenuUI.h"

#include <Engine/CTaskMgr.h>

#include <Engine/CGameObject.h>
#include <Engine/components.h>

MenuUI::MenuUI()
	: UI("Menu","##Menu")
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

        }

        if (ImGui::MenuItem("Load Level", ""))
        {

        }

        ImGui::EndMenu();
    }
}

void MenuUI::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        if (ImGui::MenuItem("Play"))
        {

        }

        if (ImGui::MenuItem("Pause"))
        {

        }

        if (ImGui::MenuItem("Stop"))
        {

        }

        ImGui::EndMenu();
    }
}

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
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}

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

        }

        ImGui::EndMenu();
    }
}


