#include "pch.h"
#include "AnimationEditorUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAnim.h>

#include "ListUI.h"

AnimationEditorUI::AnimationEditorUI()
	: UI("Animation Editor", "##AnimationEditor")
	, m_CurAtlas(nullptr)
	, m_TargetObj(nullptr)
	, m_CanvasLeftTop(ImVec2(0.f,0.f))
	, m_Scrolling(ImVec2(0.f,0.f))
	, m_MousePos(ImVec2(0.f,0.f))
	, m_CenterPos(ImVec2(0.f,0.f))
	, m_CanvasIdx(0)
	, m_AnimIdx(-1)
	, m_fps(1.f)
	, m_Wheelsz(1.f)
	, m_bSlice(false)
	, m_bTrim(false)
	, m_bGrid(false)
	, m_bSmart(false)
	, m_FrmInfo{}
{
	Deactivate();
}

AnimationEditorUI::~AnimationEditorUI()
{
}

void AnimationEditorUI::render_update()
{
	static char spritename[256] = {}; // Anim Name 입력란

	// botton 추가
	if (ImGui::Button("Load Texture"))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("Select##List");

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::TEXTURE);
		for (const auto& pair : mapAsset)
			pListUI->AddString(string(pair.first.begin(), pair.first.end()));

		pListUI->SetDbClickDelegate(this, (Delegate_1)&AnimationEditorUI::SelectAtlas);
		pListUI->Activate();
	}
	ImGui::SameLine();
	if (ImGui::Button("Slice Sprite"))
	{
		m_bSlice = true;
		m_bTrim = false;
		m_bSmart = false;
		m_bGrid = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Trim Slice"))
	{
		m_bTrim = true;
		m_bSlice = false;
		m_bSmart = false;
		m_bGrid = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Smart Slice"))
	{
		m_bSmart = true;
		m_bSlice = false;
		m_bTrim = false;
		m_bGrid = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Grid Slice"))
	{
		m_bGrid = true;
		m_bSmart = false;
		m_bSlice = false;
		m_bTrim = false;

	}
	ImGui::SameLine();
	if (ImGui::Button("Add Sprite to Animation"))
	{
		tAnimFrm frm = {};
		frm.vLeftTop = Vec2(m_vecRect[m_CanvasIdx].Min.x/ m_CurAtlas.Get()->GetWidth(), 
							m_vecRect[m_CanvasIdx].Min.y/m_CurAtlas.Get()->GetHeight());
		frm.vSlice = Vec2(m_vecRect[m_CanvasIdx].GetSize().x / m_CurAtlas.Get()->GetWidth(), 
						  m_vecRect[m_CanvasIdx].GetSize().y / m_CurAtlas.Get()->GetHeight());
		m_vecAnim.push_back(frm);
		m_AnimIdx = m_vecAnim.size() - 1;
	}

	ImGuiIO& io = ImGui::GetIO();

	ImGui::Text("Mouse pos: (%g, %g)", m_MousePos.x, m_MousePos.y);
	ImGui::Text("Center pos: (%g, %g)", m_CenterPos.x, m_CenterPos.y);
	ImGui::Text("Mouse wheel: %.1f", m_Wheelsz);

	// Atlas
	DrawCanvas();

	// animation make
	ImGui::Begin("Sprite Animation");
	if (ImGui::Button("New Animation"))
	{
		//PreView 상태 모두 초기화
		if (!m_vecAnim.empty())
			m_vecAnim.clear();

		m_AnimIdx = -1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Animation"))
	{
		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"ALL\0*.*\0Anim\0*.anim";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;

		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetContentPath();
		strInitPath += L"anim\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			LoadAnim(CPathMgr::GetRelativePath(szSelect));
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Animation"))
	{
		wchar_t szSelect[256] = { };
		OPENFILENAME ofn = {};

		char chName[256] = {};
		strcpy_s(chName,spritename);
		strncat_s(chName, ".anim", 6);
		CharToWChar(chName, szSelect);

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"ALL\0*.*\0Anim\0*.anim";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetContentPath();
		strInitPath += L"anim\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			SaveAnim(CPathMgr::GetRelativePath(szSelect));
		}
		//bgsize += 200.f;

	}
	ImGui::SameLine();
	if (ImGui::Button("Save Animator"))
	{
		// todo
		// sprite 아래 애니메이션 여러개 목록 띄워줌
		// 현재 선택된 애니메이션을 sprite로 보여줌
		// 저장시 애니메이션 묶어서 애니메이터로 save
	}

	// preview
	ImVec2 PrevCanvasLT = ImGui::GetCursorScreenPos();    
	ImVec2 Prevcanvas_sz = ImVec2(250.f,250.f);
	ImVec2 PrevCanvasRB = PrevCanvasLT + Prevcanvas_sz;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(PrevCanvasLT, PrevCanvasRB, IM_COL32(70, 70, 70, 255));
	draw_list->AddRectFilled(PrevCanvasLT, PrevCanvasRB - Prevcanvas_sz / 2, IM_COL32(50, 50, 50, 255));
	draw_list->AddRectFilled(PrevCanvasLT + Prevcanvas_sz /2, PrevCanvasRB, IM_COL32(50, 50, 50, 255));
	if (0 != m_vecAnim.size())
	{
		// Canvas 안에 이미지출력
		draw_list->PushClipRect(PrevCanvasLT, PrevCanvasRB, true);

		ImVec2 displayLT = ImVec2(m_vecAnim[m_AnimIdx].vLeftTop);
		ImVec2 displayRB = ImVec2(m_vecAnim[m_AnimIdx].vLeftTop + m_vecAnim[m_AnimIdx].vSlice);
		ImVec2 displaySize = ImVec2(m_vecAnim[m_AnimIdx].vSlice);

		ImVec2 uv0 = ImVec2(displayLT.x , displayLT.y);
		ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x) , (displayLT.y + displaySize.y));
		
		// UV -> 일반좌표계
		ImVec2 imageLT = PrevCanvasLT + (Prevcanvas_sz / 2) - ImVec2(displaySize.x * m_CurAtlas->GetWidth(), displaySize.y * m_CurAtlas->GetHeight()) / 2;
		ImVec2 imageRB = imageLT + ImVec2(displaySize.x * m_CurAtlas->GetWidth(), displaySize.y * m_CurAtlas->GetHeight());
		ImRect rec(imageLT,imageRB);

		rec.Translate(m_vecAnim[m_AnimIdx].vOffset);
		draw_list->AddImage(m_CurAtlas.Get()->GetSRV().Get(), rec.Min, rec.Max, uv0, uv1);
		draw_list->PopClipRect();
	}

	// 십자선
	draw_list->AddLine(ImVec2(PrevCanvasLT.x + Prevcanvas_sz.x/2, PrevCanvasLT.y)
					 , ImVec2(PrevCanvasLT.x + Prevcanvas_sz.x/2, PrevCanvasRB.y), IM_COL32(0, 255, 0, 150));
	draw_list->AddLine(ImVec2(PrevCanvasLT.x, PrevCanvasLT.y + Prevcanvas_sz.y/2)
					 , ImVec2(PrevCanvasRB.x, PrevCanvasLT.y + Prevcanvas_sz.y/2), IM_COL32(255, 0, 0, 150));

	//Anim 정보
	ImGui::SetCursorPosX(265.f);
	ImGui::Text("Name"); 
	ImGui::SetCursorPosX(265.f);
	ImGui::InputText("##Sprite", (char*)spritename, IM_ARRAYSIZE(spritename));
	ImGui::SetCursorPosX(265.f); ImGui::Separator();

	ImGui::SetCursorPos(ImVec2(265.f,110.f));
	ImGui::Text("Frame Number");
	ImGui::SetCursorPosX(265.f);
	static bool inputs_step = true;
	const int frmUnit = 1;
	ImGui::PushItemWidth(125);
	ImGui::InputScalar("##FrameNum", ImGuiDataType_U8, &m_AnimIdx, inputs_step ? &frmUnit : NULL, NULL, "%u");
	if (m_AnimIdx > m_vecAnim.size() - 1) m_AnimIdx = m_vecAnim.size() - 1;
	ImGui::SetCursorPosX(265.f); ImGui::Separator();
	ImGui::PopItemWidth();

	ImGui::SetCursorPos(ImVec2(265.f, 180.f));
	ImVec2 offset = ImVec2(0.f, 0.f);
	if(0 != m_vecAnim.size()) offset = ImVec2(m_vecAnim[m_AnimIdx].vOffset);
	ImGui::Text("Offset : X = %0.f, Y = %0.f", offset.x, offset.y);

	ImGui::SetCursorPos(ImVec2(335.f,212.f));
	if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) { m_vecAnim[m_AnimIdx].vOffset.y--; }
	ImGui::SetCursorPosX(313.f);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { m_vecAnim[m_AnimIdx].vOffset.x--; }
	ImGui::SameLine(); ImGui::SetCursorPosX(357.f);
	if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) { m_vecAnim[m_AnimIdx].vOffset.x++; }
	ImGui::SetCursorPosX(335.f);
	if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) { m_vecAnim[m_AnimIdx].vOffset.y++; }
	ImGui::SetCursorPos(ImVec2(265.f,300.f)); ImGui::Separator();

	ImGui::Text("Frame Length"); ImGui::SameLine();
	ImU8 frmlength = m_vecAnim.size(); // Frame num  실시간 변경 필요
	static bool length_step = true;
	ImGui::PushItemWidth(110);
	ImGui::InputScalar("##FrameLength", ImGuiDataType_U8, &frmlength, length_step ? &frmUnit : NULL, NULL, "%u");
	if (frmlength > m_vecAnim.size() - 1) frmlength = m_vecAnim.size() - 1;

	ImGui::SameLine();
	ImGui::Text("FPS"); ImGui::SameLine();
	static bool fps_step = true;
	const float fpsUnit = 1.f;
	ImGui::InputScalar("##FPS", ImGuiDataType_Float, &m_fps, fps_step ? &fpsUnit : NULL, NULL, "%0.f");
	if (m_fps <= 0) m_fps = 0.01;
	ImGui::PopItemWidth(); 

	ImGui::SameLine();
	ImGui::Text("Loop");
	static bool bLoop = true;
	ImGui::SameLine();
	ImGui::Checkbox("##Loop", &bLoop);

	static bool bPlay = false;
	static bool bPause = false;
	static bool bfinish = false;
	static float ftime = 0.f;
	if (ImGui::Button("Play")) 
	{
		bPlay = true;
		bfinish = false;
		if (bPause)
			bPause = false;
		else
		{ 
			m_AnimIdx = 0; 
			ftime = 0.f;
		}
	}
	ImGui::SameLine();
	if (bPlay && ImGui::Button("Pause"))
	{
		bPause = true;
		bPlay = false;
	}
	ImGui::SameLine();
	ImGui::Text("Current Frame : %d", m_AnimIdx);

	if (m_TargetObj)
	{
		ImGui::SameLine(); 
		if (ImGui::Button("Apply"))
		{
			wchar_t wName[256] = { };
			char chName[256] = {};
			strcpy_s(chName, spritename);
			CharToWChar(chName, wName);

			// BackGround 세팅
			ImVec2 MaxSZ = ImVec2(0.f, 0.f);
			for (int i = 0; i < m_vecAnim.size(); i++)
			{
				ImVec2 Size = ImVec2(m_vecAnim[i].vSlice);
				if (Size.x > MaxSZ.x) MaxSZ.x = Size.x;
				if (Size.y > MaxSZ.y) MaxSZ.y = Size.y;
				m_vecAnim[i].vBackground = Vec2(MaxSZ.x + 100.f / m_CurAtlas->GetWidth(),
												MaxSZ.y + 100.f / m_CurAtlas->GetHeight());
				m_vecAnim[i].Duration = 1.f / m_fps;
			}

			m_TargetObj->Animator2D()->Create(wName, m_CurAtlas, m_vecAnim, m_vecAnim.size());
		}
	}
	
	if (bPlay)
	{
		ftime += DT_ENGINE;
		if (1.f / m_fps < ftime)
		{
			++m_AnimIdx;
			if (m_vecAnim.size() <= m_AnimIdx) // frmlength 연결필요
			{
				m_AnimIdx = (int)m_vecAnim.size() - 1;
				bfinish = true;
			}
			ftime = 0.f;
		}

		if (bfinish && bLoop)
		{
			m_AnimIdx = 0;
			bfinish = false;
		}
	}

	// Anim 나열
	ImGui::BeginChild("child", ImVec2(ImGui::GetContentRegionAvail().x, 125.f), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar| ImGuiWindowFlags_NoScrollWithMouse);
	if (nullptr != m_CurAtlas)
	{
		ImVec2 MaxSZ = ImVec2(0.f, 0.f);
		for (int i = 0; i < m_vecAnim.size(); i++)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			ImGuiContext& g = *GImGui;
			const ImVec2 padding = g.Style.FramePadding;

			ImVec4 col = ImVec4(1, 1, 1, 1);
			ImVec2 displayLT = ImVec2(m_vecAnim[i].vLeftTop);
			ImVec2 displayRB = ImVec2(m_vecAnim[i].vLeftTop + m_vecAnim[i].vSlice);
			ImVec2 displaySize = ImVec2(m_vecAnim[i].vSlice);

			ImVec2 uv0 = ImVec2(displayLT.x , displayLT.y);
			ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x), (displayLT.y + displaySize.y));

			// rect 선택
			const ImRect select(window->DC.CursorPos, window->DC.CursorPos + ImVec2(100.f, 100.f) + padding * 2.0f);
			if (select.Contains(io.MousePos) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				m_AnimIdx = i;

			if (i == m_AnimIdx) // 선택된 rect 테두리 색상
				col = ImVec4(1, 0, 0, 1);

			//ImVec2 imageLT = PrevCanvasLT + (Prevcanvas_sz / 2) - ImVec2(displaySize.x * m_CurSheet->GetWidth(), displaySize.y * m_CurSheet->GetHeight()) / 2;
			//ImVec2 imageRB = imageLT + ImVec2(displaySize.x * m_CurSheet->GetWidth(), displaySize.y * m_CurSheet->GetHeight());
			//ImRect rec(imageLT, imageRB);

			ImGui::Image(m_CurAtlas.Get()->GetSRV().Get(), ImVec2(100.f, 100.f), uv0, uv1, ImVec4(1, 1, 1, 1), col);
			ImGui::SameLine();
		}
	}
	ImGui::SetScrollX(ImGui::GetScrollX() - (15.f * io.MouseWheel));
	ImGui::EndChild();

	// 더미 Target Animation List
	vector<string> vAnim;
	ImVector<char*> sprite;
	if (nullptr == m_TargetObj)
	{
		char ch[] = "Nothing";
		sprite.push_back(ch);
	}
	else
	{
		if (0 == m_TargetObj->Animator2D()->GetAnimCount())
		{
			char ch[] = "Nothing";
			sprite.push_back(ch);
		}
		else
		{
			char ch[] = "Select";
			sprite.push_back(ch);

			m_TargetObj->Animator2D()->GetAnimName(vAnim);
			for (size_t i = 0; i < vAnim.size(); i++)
			{
				char* pCh = const_cast<char*>(vAnim[i].c_str());
				sprite.push_back(pCh);
			}
		}
	}
	static int sprite_idx = 0;
	const char* combo_preview_value = sprite[sprite_idx];

	ImGui::Text("Select Anim "); ImGui::SameLine();
	if (ImGui::BeginCombo("##SelectSprite", combo_preview_value))
	{
		for (int n = 0; n < sprite.size(); n++)
		{
			const bool is_selected = (sprite_idx == n);
			if (ImGui::Selectable(sprite[n], is_selected))
			{
				sprite_idx = n;
				SelectSprite(sprite[sprite_idx]);
			}

			// 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}

void AnimationEditorUI::DrawCanvas()
{
	// canvas
	static ImVector<ImVec2> points;
	static ImVec2 scrolling(0.0f, 0.0f);
	static float WheelSz = 1.f;
	static bool opt_enable_grid = true;
	static bool cutting_rect = false;
	static bool opt_enable_context_menu = true;

	ImGui::Checkbox("Enable grid", &opt_enable_grid);
	ImGui::SameLine();
	ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);

	static BYTE grid_row = 1, grid_col = 1;
	static UINT grid_width = 1, grid_height = 1;
	if (m_bGrid && nullptr != m_CurAtlas)
	{
		ImGui::PushItemWidth(50);
		ImGui::SameLine(350.f);
		ImGui::Text("Row"); ImGui::SameLine();
		ImGui::DragScalar("##Row", ImGuiDataType_U8, &grid_row);
		ImGui::SameLine();
		ImGui::Text("Col"); ImGui::SameLine();
		ImGui::DragScalar("##Col", ImGuiDataType_U8, &grid_col);
		ImGui::SameLine();
		ImGui::Text("Width"); ImGui::SameLine();
		ImGui::DragScalar("##width", ImGuiDataType_U32, &grid_width);
		ImGui::SameLine();
		ImGui::Text("Height"); ImGui::SameLine();
		ImGui::DragScalar("##height", ImGuiDataType_U32, &grid_height);
		ImGui::PopItemWidth();
		if (grid_row == 0) grid_row = 1;
		if (grid_col == 0) grid_col = 1;
	}

	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail() ;  
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	m_CanvasLeftTop = canvas_p0;
	m_CenterPos = canvas_sz / 2;
	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	m_CenterPos -= scrolling;
	ImVec2 WheelOffset = m_CenterPos * WheelSz - m_CenterPos;
	const ImVec2 origin(canvas_p0.x + scrolling.x - WheelOffset.x, canvas_p0.y + scrolling.y - WheelOffset.y);
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	m_MousePos = mouse_pos_in_canvas;
	
	// Canvas 안에 이미지출력
	if (nullptr != m_CurAtlas)
	{
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);
		ComPtr<ID3D11ShaderResourceView> my_texture = NULL;

		my_texture = m_CurAtlas.Get()->GetSRV().Get();
		float my_image_width = m_CurAtlas.Get()->GetWidth() * WheelSz;
		float my_image_height = m_CurAtlas.Get()->GetHeight() * WheelSz;

		ImVec2 left_top = m_CanvasLeftTop + scrolling - WheelOffset;
		ImVec2 right_bottom = (left_top + ImVec2(my_image_width, my_image_height));

		draw_list->AddImage((void*)my_texture.Get(), left_top, right_bottom);
		draw_list->AddRect(left_top, right_bottom, IM_COL32(255, 255, 255, 255)); // 아틀라스테두리
		draw_list->PopClipRect();
	}

	// 좌클릭시
	if (ImGui::IsWindowFocused() && m_bSlice && is_hovered && !cutting_rect && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		points.push_back(mouse_pos_in_canvas / WheelSz);
		points.push_back(mouse_pos_in_canvas / WheelSz);

		cutting_rect = true;
	}
	if (cutting_rect)
	{
		points.back() = mouse_pos_in_canvas / WheelSz;

		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			cutting_rect = false;
			ImRect rect(points[points.size() - 2],points.back());
			if (rect.Min.x > rect.Max.x) ImSwap(rect.Min.x, rect.Max.x);
			if (rect.Min.y > rect.Max.y) ImSwap(rect.Min.y, rect.Max.y);
			m_vecRect.push_back(rect);
			m_bSlice = false;
		}
	}

	// 오른쪽 마우스 드래그
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (ImGui::IsWindowFocused() && is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
		m_Scrolling = scrolling;
	}

	// 마우스 휠
	if (ImGui::IsWindowFocused() && (io.MouseWheel > 0.f || io.MouseWheel))
	{
		ImRect Incanvas(canvas_p0, canvas_p1);
		if (Incanvas.Contains(io.MousePos))
		{
			WheelSz += 0.1f * io.MouseWheel;
			if (WheelSz <= 0.f) WheelSz = 0.f;
			m_Wheelsz = WheelSz;
		}
	}

	// Draw grid 
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x , GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	} 

	if (m_bSmart)
	{
		SmartSlice(points);
	}

	if (m_bGrid)
	{
		GridSlice(points,grid_row,grid_col,grid_width,grid_height);
	}

	// draw rect
	//static int selectidx = -1;	 // 나중에 여러 개 선택되게
	for (int n = 0; n < points.Size; n += 2)
	{
		ImGuiContext& g = *GImGui;
		const ImVec2 padding = g.Style.FramePadding;
		ImU32 col = IM_COL32(255, 255, 0, 255);

		ImVec2 leftTop = points[n] * WheelSz + origin;
		ImVec2 rightBottom  = points[n+1] * WheelSz + origin;

		if (leftTop.x > rightBottom.x) ImSwap(leftTop.x, rightBottom.x);
		if (leftTop.y > rightBottom.y) ImSwap(leftTop.y, rightBottom.y);


		const ImRect select(leftTop, rightBottom + padding * 2.0f);
		if (ImGui::IsWindowFocused() && select.Contains(io.MousePos) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			m_CanvasIdx = n / 2;

		// MouseGrip
		MouseGrip(io.MousePos, mouse_pos_in_canvas, leftTop, rightBottom, points, n, WheelSz);

		if (n / 2  == m_CanvasIdx)
		{
			col = IM_COL32(255, 0, 0, 255);

			if (m_bTrim)
			{
				m_bTrim = false;
				ImRect rec = TrimAtlas(m_CanvasIdx);

				points[n] = rec.Min;
				points[n + 1] = rec.Max;
				m_vecRect[n / 2] = rec;
			}
		}
		
		draw_list->AddRect(leftTop ,rightBottom , col, 2.0f);
	}

	draw_list->PopClipRect();
}

void AnimationEditorUI::SelectAtlas(DWORD_PTR _ptr)
{
	string strAnim = (char*)_ptr;
	wstring strAnimName = ToWString(strAnim);

	Ptr<CTexture> Tex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAnimName);
	m_CurAtlas = Tex;
}

void AnimationEditorUI::SelectSprite(char* _anim)
{
	wstring str = ToWString(_anim);

	if (L"Nothing" == str || L"Select" == str)
		return;

	// Anim을 선택했는데 없으면 assert
	assert(m_TargetObj->Animator2D()->FindAnim(str));

	CAnim* sprite = m_TargetObj->Animator2D()->FindAnim(str);
	vector<tAnimFrm> animfrm = sprite->GetAnimFrm();
	m_CurAtlas = sprite->GetAtalsTex();
	m_vecAnim = animfrm;
	//m_vecAnim.assign(animfrm.begin(), animfrm.end());
	m_AnimIdx = 0;
}

void AnimationEditorUI::MouseGrip(const ImVec2& _ioMousePos, const ImVec2& _canvasMousePos,
	const ImVec2& _LT, const ImVec2& _RB, ImVector<ImVec2>& _points, const float _idx, const float _wheel )
{
	if (!ImGui::IsWindowFocused())
		return;

	// Mouse Grip Cursur
	// 왼쪽
	if (_LT.x - 10.f < _ioMousePos.x &&
		_LT.x + 10.f > _ioMousePos.x &&
		_LT.y <= _ioMousePos.y &&
		_RB.y > _ioMousePos.y)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx].x = _canvasMousePos.x / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Min.x = _points[_idx].x;
			}
		}

	}

	// 오른쪽
	if (_RB.x - 10.f < _ioMousePos.x &&
		_RB.x + 10.f > _ioMousePos.x &&
		_LT.y <= _ioMousePos.y &&
		_RB.y > _ioMousePos.y)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx + 1].x = _canvasMousePos.x / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Max.x = _points[_idx + 1].x;
			}
		}
	}

	// 위
	if (_LT.y - 10.f < _ioMousePos.y &&
		_LT.y + 10.f > _ioMousePos.y &&
		_LT.x <= _ioMousePos.x &&
		_RB.x > _ioMousePos.x)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx].y = _canvasMousePos.y / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Min.y = _points[_idx].y;
			}
		}
	}

	// 아래
	if (_RB.y - 10.f < _ioMousePos.y &&
		_RB.y + 10.f > _ioMousePos.y &&
		_LT.x <= _ioMousePos.x &&
		_RB.x > _ioMousePos.x)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		static bool grip_resize = false;
		if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			grip_resize = true;

		if (grip_resize)
		{
			_points[_idx + 1].y = _canvasMousePos.y / _wheel;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
			{
				grip_resize = false;
				m_vecRect[_idx / 2].Max.y = _points[_idx + 1].y;
			}
		}
	}
}



ImRect AnimationEditorUI::TrimAtlas(int _idx)
{
	ImRect rec = m_vecRect[_idx];
	int rec_width = rec.GetWidth();
	int rec_height = rec.GetHeight();
	const int atlas_width = m_CurAtlas->GetWidth();
	const int atlas_height = m_CurAtlas->GetHeight();

	// slice rec가 atlas범위를 벗어날경우
	if (rec.Min.x < 0) { rec_width += rec.Min.x; rec.Min.x = 0; }
	if (rec.Min.y < 0) { rec_height += rec.Min.y; rec.Min.y = 0; }
	if (rec.Max.x > atlas_width) rec_width = atlas_width - rec.Min.x;
	if (rec.Max.y > atlas_height) rec_height = atlas_height - rec.Min.y;

	ImVec2 leftTop = ImVec2(atlas_width + 1.f, atlas_height + 1.f);
	ImVec2 rightBottom = ImVec2(-1.f, -1.f);

	tPixel* pPixel = m_CurAtlas.Get()->GetPixels(); 
	for (int i = 0; i < rec_height; i++)
	{
		for (int j = 0; j < rec_width; j++)
		{
			int x = rec.Min.x + j;
			int y = rec.Min.y + i;
			tPixel pixel = pPixel[(atlas_width * y) + x];
			if (0 != pixel.a)
			{
				if (x < leftTop.x) leftTop.x = x;
				if (y < leftTop.y) leftTop.y = y;
			}
			else if (0 != pPixel[(atlas_width * y) + x - 1].a ||
					(0 != y && 0 != pPixel[(atlas_width * (y - 1)) + x].a))
			{
				if (x > rightBottom.x) rightBottom.x = x;
				if (y > rightBottom.y) rightBottom.y = y;
			}
		}
	}

	rec.Min = leftTop;
	rec.Max = rightBottom;

	return rec;
}
void AnimationEditorUI::GridSlice(ImVector<ImVec2>& _points, BYTE& _row, BYTE& _col, UINT& _width, UINT& _height)
{
	if (nullptr == m_CurAtlas)
		return;

	if (!_points.empty() && !m_vecRect.empty())
	{
		_points.clear(); // 기존 points vector를 비워줌
		m_vecRect.clear();
	}

	_width = m_CurAtlas->GetWidth() / _col;
	_height = m_CurAtlas->GetHeight() / _row;

	for (size_t i = 0; i < _row; i++)
	{
		for (size_t j = 0; j < _col; j++)
		{
			_points.push_back(ImVec2(_width * j, _height * i));
			_points.push_back(ImVec2(_width * (j + 1), _height * (i + 1)));
			m_vecRect.push_back(ImRect(ImVec2(_width * j, _height * i), ImVec2(_width * (j + 1), _height * (i + 1))));
		}
	}
}
void AnimationEditorUI::SmartSlice(ImVector<ImVec2>& _points)
{
	m_bSmart = false;

	if (nullptr == m_CurAtlas)
		return;
	
	if(!_points.empty())
		_points.clear(); // 기존 points vector를 비워줌

	tPixel* pPixel = m_CurAtlas.Get()->GetPixels();
	const int width = m_CurAtlas->GetWidth();
	const int height = m_CurAtlas->GetHeight();
	vector<vector<bool>> visit(height,vector<bool>(width,false)); // 방문 pixel

	const int dir[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // 상 하 좌 우

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (visit[y][x])
				continue;

			ImVec2 leftTop = ImVec2(m_CurAtlas->GetWidth() + 1.f, m_CurAtlas->GetHeight() + 1.f);
			ImVec2 rightBottom = ImVec2(-1.f, -1.f);

			tPixel pixel = pPixel[width * y + x];
			if (0 != pixel.a)
			{
				queue<ImVec2> qPixel;
				qPixel.push(ImVec2(x, y));
				while (!qPixel.empty())
				{
					ImVec2 dot = qPixel.front();
					visit[dot.y][dot.x] = true;

					for (int i = 0; i < 4; i++)
					{
						int newX = dot.x + dir[i][0];
						int newY = dot.y + dir[i][1];

						if (dot.x < leftTop.x) leftTop.x = dot.x;
						if (dot.y < leftTop.y) leftTop.y = dot.y;
						if (newX > rightBottom.x) rightBottom.x = newX;
						if (newY > rightBottom.y) rightBottom.y = newY;

						if (newX < 0 || newX >= width || newY < 0 || newY >= height)
							continue;

						tPixel pixel = pPixel[width * newY + newX];
						if (0 != pixel.a && !visit[newY][newX])
						{
							qPixel.push(ImVec2(newX, newY));
						}

						visit[newY][newX] = true;
					}
					qPixel.pop();
				}
				// LT,RB points에 저장
				_points.push_back(leftTop);
				_points.push_back(rightBottom);
				m_vecRect.push_back(ImRect(leftTop, rightBottom));
			}
		}
	}

}

void AnimationEditorUI::SaveAnim(const wstring& _str)
{
	CAnim* pAnim = new CAnim;
	pAnim->Create(_str, nullptr, m_CurAtlas, m_vecAnim, m_vecAnim.size());

	// anim 을 저장할 경로
	wstring strAnimPath = CPathMgr::GetContentPath();
	strAnimPath += _str;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strAnimPath.c_str(), L"wb");

	// background 세팅
	ImVec2 MaxSZ = ImVec2(0.f, 0.f);
	for (int i = 0; i < m_vecAnim.size(); i++)
	{
		ImVec2 Size = ImVec2(m_vecAnim[i].vSlice);
		if (Size.x > MaxSZ.x) MaxSZ.x = Size.x;
		if (Size.y > MaxSZ.y) MaxSZ.y = Size.y;
		m_vecAnim[i].vBackground = Vec2(MaxSZ.x + 100.f / m_CurAtlas->GetWidth(),
			MaxSZ.y + 100.f / m_CurAtlas->GetHeight());
		m_vecAnim[i].Duration = 1.f / m_fps;
	}

	pAnim->SaveToFile(pFile);

	fclose(pFile);

	delete pAnim;
}

void AnimationEditorUI::LoadAnim(const wstring& _str)
{
	CAnim* pAnim = nullptr;

	// anim 을 불러올 경로
	wstring strAnimPath = CPathMgr::GetContentPath();
	strAnimPath += _str;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strAnimPath.c_str(), L"rb");

	// anim의 이름을 읽는다.
	pAnim = new CAnim;
	pAnim->LoadFromFile(pFile);

	if (!m_vecAnim.empty())
		m_vecAnim.clear();
	m_AnimIdx = 0;

	m_CurAtlas = pAnim->GetAtalsTex();
	m_vecAnim = pAnim->GetAnimFrm();
	m_fps = 1.f / m_vecAnim[0].Duration;
	fclose(pFile);

	delete pAnim;
}

void AnimationEditorUI::Deactivate()
{
	UI::Deactivate();
}

	// 오른쪽 마우스 메뉴
//ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
//if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
//ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
//if (ImGui::BeginPopup("context"))
//{
//	if (cutting_rect)
//		points.resize(points.size() - 2);
//	cutting_rect = false;
//	if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0))
//	{
//		points.resize(points.size() - 2);
//	}
//	if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0))
//	{
//
//		points.clear();
//	}
//	ImGui::EndPopup();
//}

// 줄에 걸치면 ImGui::IsItemHovered() ImGui::SetMouseCursor(ImGuiMouseCursor_COUNT)

