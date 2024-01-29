#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Inspector.h"
#include "Content.h"
#include "Outliner.h"
#include "ListUI.h"
#include "AnimationEditorUI.h"

CImGuiMgr::CImGuiMgr()
    : m_bDemoUI(true)
    , m_Open(true)
{
}

CImGuiMgr::~CImGuiMgr()
{
    // ImGui Clear       
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // UI 
    Delete_Map(m_mapUI);
}

void CImGuiMgr::init(HWND _hMainWnd, ComPtr<ID3D11Device> _Device
    , ComPtr<ID3D11DeviceContext> _Context)
{
    // ImGui ÃÊ±âÈ­
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hMainWnd);
    ImGui_ImplDX11_Init(_Device.Get(), _Context.Get());

    create_ui();

    //CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    //CGameObject* pObject = pCurLevel->FindObjectByName(L"Player");
    //((Inspector*)FindUI("##Inspector"))->SetTargetObject(pObject);
}

void CImGuiMgr::progress()
{
    tick();

    render();
}

void CImGuiMgr::tick()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    if (m_Open)
        dockspace();
    
    if (m_bDemoUI)
    {
        ImGui::ShowDemoWindow(&m_bDemoUI);
    }

    for (const auto& pair : m_mapUI)
    {
        pair.second->tick();
    }
}

void CImGuiMgr::render()
{
    for (const auto& pair : m_mapUI)
    {
        pair.second->render();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}


UI* CImGuiMgr::FindUI(const string& _strUIName)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_strUIName);

    if (iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}

void CImGuiMgr::AddUI(const string& _strKey, UI* _UI)
{
    UI* pUI = FindUI(_strKey);
    assert(!pUI);
    m_mapUI.insert(make_pair(_strKey, _UI));
}

void CImGuiMgr::dockspace()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Close", NULL, false, &m_Open != NULL))
                m_Open = false;

            ImGui::Separator();
            ImGui::EndMenu();

        }
        if (ImGui::BeginMenu("Scene"))
        {
            // ToDo
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tool"))
        {
            if (ImGui::MenuItem("Animation Editor"))
            {
                //ToDo
                AnimationEditorUI* Editor = (AnimationEditorUI*)CImGuiMgr::GetInst()->FindUI("##AnimationEditor");

                Editor->Activate();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Map Editor"))
            {
                //ToDo
            }
            ImGui::Separator();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void CImGuiMgr::create_ui()
{
    UI* pUI = nullptr;

    // Inspector
    pUI = new Inspector;
    AddUI(pUI->GetID(), pUI);

    // Content
    pUI = new Content;
    AddUI(pUI->GetID(), pUI);

    // Outliner
    pUI = new Outliner;
    AddUI(pUI->GetID(), pUI);

    // List
    pUI = new ListUI;           
    AddUI(pUI->GetID(), pUI);  

    // AnimationEditor
    pUI = new AnimationEditorUI;
    AddUI(pUI->GetID(), pUI);
}





