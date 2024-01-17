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


CImGuiMgr::CImGuiMgr()
    : m_bDemoUI(true)
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
	// ImGui 초기화
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

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    create_ui();

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    CGameObject* pObject = pCurLevel->FindObjectByName(L"Player");
    ((Inspector*)FindUI("##Inspector"))->SetTargetObject(pObject);
}

void CImGuiMgr::progress()
{
    tick();

    render();
}

void CImGuiMgr::tick()
{
    // ImGui Update & Rendering
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // DemoUI 생성
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
}


// Sample
//bool show_demo_window = true;
//bool show_another_window = false;
//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//if (show_demo_window)
//ImGui::ShowDemoWindow(&show_demo_window);
//
//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//{
//    static float f = 0.0f;
//    static int counter = 0;
//
//    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//    ImGui::Checkbox("Another Window", &show_another_window);
//
//    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//        counter++;
//    ImGui::SameLine();
//    ImGui::Text("counter = %d", counter);
//
//    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//    ImGui::End();
//}
//
//// 3. Show another simple window.
//if (show_another_window)
//{
//    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//    ImGui::Text("Hello from another window!");
//    if (ImGui::Button("Close Me"))
//        show_another_window = false;
//    ImGui::End();
//}