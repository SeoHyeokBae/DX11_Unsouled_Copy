#include "pch.h"
#include "AnimationEditorUI.h"

#include <Engine/CAssetMgr.h>

#include "ListUI.h"

AnimationEditorUI::AnimationEditorUI()
	: UI("Animation Editor", "##AnimationEditor")
	, m_bOpen(true)
	, m_CurAtlas(nullptr)
	, m_CanvasLeftTop(ImVec2(0.f,0.f))
	, m_Scrolling(ImVec2(0.f,0.f))
	, m_MousePos(ImVec2(0.f,0.f))
	, m_CenterPos(ImVec2(0.f,0.f))
	, m_Wheelsz(1.f)
	, m_bSlice(false)
	, m_bTrim(false)
	, m_bSmartSlice(false)
{
	Deactivate();
}

AnimationEditorUI::~AnimationEditorUI()
{
}

void AnimationEditorUI::render_update()
{
	// botton 추가
	if (ImGui::Button("Load Texture"))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("Select##List");

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::TEXTURE);
		for (const auto& pair : mapAsset)
			pListUI->AddString(string(pair.first.begin(), pair.first.end()));

		pListUI->SetDbClickDelegate(this, (Delegate_1)&AnimationEditorUI::AtlasSelect);
		pListUI->Activate();
	}
	ImGui::SameLine();
	if (ImGui::Button("Slice Sprite"))
	{
		m_bSlice = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Trim Slice"))
	{
		m_bTrim = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Smart Slice"))
	{
		m_bSmartSlice = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Sprite to Animation"))
	{
		m_vecAnimRect.push_back(m_vecRect.back());
	}

	ImGuiIO& io = ImGui::GetIO();

	ImGui::Text("Mouse pos: (%g, %g)", m_MousePos.x, m_MousePos.y);
	ImGui::Text("Center pos: (%g, %g)", m_CenterPos.x, m_CenterPos.y);
	ImGui::Text("Mouse wheel: %.1f", m_Wheelsz);
	DrawCanvas();


	// animation make
	ImGui::Begin("Sprite Animation" , &m_bOpen );
	if (ImGui::Button("New Animation"))
	{
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Animation"))
	{
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Animation"))
	{
		// todo
	}

	// preview
	static int selectedidx = 0;
	ImVec2 SpriteCanvasLT = ImGui::GetCursorScreenPos();    
	ImVec2 Spritecanvas_sz = ImVec2(250.f,250.f);
	ImVec2 SpriteCanvasRB = ImVec2(SpriteCanvasLT.x + Spritecanvas_sz.x, SpriteCanvasLT.y + Spritecanvas_sz.y);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(SpriteCanvasLT, SpriteCanvasRB, IM_COL32(70, 70, 70, 255));
	draw_list->AddRectFilled(SpriteCanvasLT, SpriteCanvasRB - Spritecanvas_sz / 2, IM_COL32(50, 50, 50, 255));
	draw_list->AddRectFilled(SpriteCanvasLT + Spritecanvas_sz /2, SpriteCanvasRB, IM_COL32(50, 50, 50, 255));
	if (0 != m_vecAnimRect.size())
	{
		// Canvas 안에 이미지출력
		draw_list->PushClipRect(SpriteCanvasLT, SpriteCanvasRB, true);

		ImVec2 displayLT = m_vecAnimRect[selectedidx].GetTL();
		ImVec2 displayRB = m_vecAnimRect[selectedidx].GetBR();
		ImVec2 displaySize = m_vecAnimRect[selectedidx].GetSize();
		float texturewidth = (m_CurAtlas.Get()->GetWidth()) * 0.6f;
		float textureheight = (m_CurAtlas.Get()->GetHeight()) * 0.6f;

		if (m_vecAnimRect[selectedidx].Min.x > m_vecAnimRect[selectedidx].Max.x) 
			ImSwap(m_vecAnimRect[selectedidx].Min.x, m_vecAnimRect[selectedidx].Max.x);
		if (m_vecAnimRect[selectedidx].Min.y > m_vecAnimRect[selectedidx].Max.y) 
			ImSwap(m_vecAnimRect[selectedidx].Min.y, m_vecAnimRect[selectedidx].Max.y);

		ImVec2 uv0 = ImVec2(displayLT.x / texturewidth, displayLT.y / textureheight);
		ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x) / texturewidth, (displayLT.y + displaySize.y) / textureheight);

		ImRect rec(SpriteCanvasLT, SpriteCanvasRB);
		rec.Expand(-50.f);
		draw_list->AddImage(m_CurAtlas.Get()->GetSRV().Get(), rec.GetTL(), rec.GetBR(), uv0, uv1);
		draw_list->PopClipRect();
	}

	// 십자선
	draw_list->AddLine(ImVec2(SpriteCanvasLT.x + Spritecanvas_sz.x/2, SpriteCanvasLT.y)
					 , ImVec2(SpriteCanvasLT.x + Spritecanvas_sz.x/2, SpriteCanvasRB.y), IM_COL32(0, 255, 0, 150));
	draw_list->AddLine(ImVec2(SpriteCanvasLT.x, SpriteCanvasLT.y + Spritecanvas_sz.y/2)
					 , ImVec2(SpriteCanvasRB.x, SpriteCanvasLT.y + Spritecanvas_sz.y/2), IM_COL32(255, 0, 0, 150));

	// Sprite 나열
	ImGui::Dummy(ImVec2(0.f, 500.f));
	ImGui::BeginChild("child", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
	if (nullptr != m_CurAtlas)
	{

		for (int i = 0; i < m_vecAnimRect.size(); i++)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			ImGuiContext& g = *GImGui;
			const ImVec2 padding = g.Style.FramePadding;

			ImVec4 col = ImVec4(1, 1, 1, 1);

			ImVec2 displayLT = m_vecAnimRect[i].GetTL();
			ImVec2 displayRB = m_vecAnimRect[i].GetBR();
			ImVec2 displaySize = m_vecAnimRect[i].GetSize();
			float texturewidth = (m_CurAtlas.Get()->GetWidth()) * 0.6f;
			float textureheight = (m_CurAtlas.Get()->GetHeight()) * 0.6f;

			if (m_vecAnimRect[i].Min.x > m_vecAnimRect[i].Max.x) ImSwap(m_vecAnimRect[i].Min.x, m_vecAnimRect[i].Max.x);
			if (m_vecAnimRect[i].Min.y > m_vecAnimRect[i].Max.y) ImSwap(m_vecAnimRect[i].Min.y, m_vecAnimRect[i].Max.y);

			ImVec2 uv0 = ImVec2(displayLT.x / texturewidth, displayLT.y / textureheight);
			ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x) / texturewidth, (displayLT.y + displaySize.y) / textureheight);

			// rect 선택
			const ImRect select(window->DC.CursorPos, window->DC.CursorPos + m_vecRect[i].GetSize() + padding * 2.0f);
			if (select.Contains(io.MousePos) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				selectedidx = i;

			if (i == selectedidx) // 선택된 rect 테두리 색상
				col = ImVec4(1, 0, 0, 1);


			ImGui::Image(m_CurAtlas.Get()->GetSRV().Get(), ImVec2(100.f, 100.f), uv0, uv1, ImVec4(1, 1, 1, 1), col);
			//ImGui::ImageButton(m_CurAtlas.Get()->GetSRV().Get(), ImVec2(100.f, 100.f), uv0, uv1,-1, ImVec4(1, 1, 1, 0), col);
			ImGui::SameLine();
		}
	}
	ImGui::SetScrollX(ImGui::GetScrollX() - (15.f * io.MouseWheel));
	ImGui::EndChild();
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
		float my_image_width = m_CurAtlas.Get()->GetWidth() * 0.6f * WheelSz;
		float my_image_height = m_CurAtlas.Get()->GetHeight() * 0.6f * WheelSz;

		ImVec2 left_top = m_CanvasLeftTop + ImVec2(scrolling.x, scrolling.y) - WheelOffset;
		ImVec2 right_bottom = (left_top + ImVec2(my_image_width, my_image_height));

		draw_list->AddImage((void*)my_texture.Get(), left_top, right_bottom);
		draw_list->AddRect(left_top, right_bottom, IM_COL32(255, 255, 255, 255)); // 아틀라스테두리
		draw_list->PopClipRect();
	}

	// 좌클릭시
	if (m_bSlice && is_hovered && !cutting_rect && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
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
			m_vecRect.push_back(rect);
			m_bSlice = false;
		}
	}

	// 오른쪽 마우스 드래그
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
		m_Scrolling = scrolling;
	}

	// 마우스 휠
	if (io.MouseWheel > 0.f || io.MouseWheel)
	{
		ImRect Incanvas(canvas_p0, canvas_p1);
		if (Incanvas.Contains(io.MousePos))
		{
			WheelSz += 0.1f * io.MouseWheel;
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

	// draw rect
	static int selectidx = -1;	 // 나중에 여러 개 선택되게
	for (int n = 0; n < points.Size; n += 2)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiContext& g = *GImGui;
		const ImVec2 padding = g.Style.FramePadding;
		ImU32 col = IM_COL32(255, 255, 0, 255);

		ImVec2 leftTop = points[n] * WheelSz + origin;
		ImVec2 rightBottom  = points[n+1] * WheelSz + origin;

		if (leftTop.x > rightBottom.x)
		{
			leftTop.x = points[n + 1].x * WheelSz + origin.x;
			rightBottom.x = points[n].x * WheelSz + origin.x;
		}
		if (leftTop.y > rightBottom.y)
		{
			leftTop.y = points[n + 1].y * WheelSz + origin.y;
			rightBottom.y = points[n].y * WheelSz + origin.y;
		}

		const ImRect select(leftTop, rightBottom + padding * 2.0f);

		// Mouse Grip Cursur
		static ImGuiDir dir = ImGuiDir_None;
		static bool grip_resize = false;
		static ImVec2 grip_point;
		static ImVec2 grip_delta = ImVec2(0.f,0.f);
			if (leftTop.x -5.f <= io.MousePos.x &&
				leftTop.x + 5.f > io.MousePos.x && 
				leftTop.y <= io.MousePos.y &&
				rightBottom.y > io.MousePos.y)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

				if (!grip_resize && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					grip_point = mouse_pos_in_canvas / WheelSz;
					grip_resize = true;
				}
				if (grip_resize)
				{
					grip_delta = grip_point - mouse_pos_in_canvas / WheelSz;
					leftTop.x -= grip_delta.x;
					points[n].x = (leftTop.x - origin.x) / WheelSz;
					if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
					{
						grip_resize = false;
					}
				}
			}

			if (rightBottom.x - 5.f <= io.MousePos.x &&
				rightBottom.x + 5.f > io.MousePos.x &&
				leftTop.y <= io.MousePos.y &&
				rightBottom.y > io.MousePos.y)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				dir = ImGuiDir_Right;
			}

			if (leftTop.y - 5.f <= io.MousePos.y &&
				leftTop.y + 5.f > io.MousePos.y &&
				leftTop.x <= io.MousePos.x &&
				rightBottom.x > io.MousePos.x)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				dir = ImGuiDir_Up;
			}

			if (rightBottom.y - 5.f <= io.MousePos.y &&
				rightBottom.y + 5.f > io.MousePos.y &&
				leftTop.x <= io.MousePos.x &&
				rightBottom.x > io.MousePos.x)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				dir = ImGuiDir_Down;
			}

		if (select.Contains(io.MousePos) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			selectidx = n;

		if(n == selectidx)
			col = IM_COL32(255, 0, 0, 255);
		
		draw_list->AddRect(leftTop ,rightBottom , col, 2.0f);
	}

	draw_list->PopClipRect();
}

void AnimationEditorUI::AtlasSelect(DWORD_PTR _ptr)
{
	string strAnim = (char*)_ptr;
	wstring strAnimName = ToWString(strAnim);

	Ptr<CTexture> Tex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAnimName);
	m_CurAtlas = Tex;
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