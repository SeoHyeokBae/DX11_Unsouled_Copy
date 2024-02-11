#include "pch.h"
#include "AnimationEditorUI.h"
#include "imgui_internal.h"

#include <Engine/CAssetMgr.h>


AnimationEditorUI::AnimationEditorUI()
	: UI("Animation Editor", "##AnimationEditor")
	, m_bOpen(true)
	, m_CurAtlas(nullptr)
	, m_CanVasLeftTop(ImVec2(0.f,0.f))
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
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Slice Sprite"))
	{
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Trim Slice"))
	{
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Smart Slice"))
	{
		// todo
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Sprite to Animation"))
	{
		// todo
	}

	DrawCanvas();

	ImGui::Begin("list" , &m_bOpen );
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();    
	ImVec2 canvas_sz = ImVec2(250.f,250.f);
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	ImGui::End();

	// Sprite 나열
	ImGui::Begin("Sprite", &m_bOpen);
	ImGui::BeginChild("child", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	if (nullptr != m_CurAtlas)
	{
		for (int i = 0; i < m_vecRect.size(); i++)
		{
			if ( 0 != i)
				ImGui::SameLine(110.f * i);

			ImVec2 displayLT = m_vecRect[i].GetTL() + m_CanVasLeftTop;
			ImVec2 displayRB = m_vecRect[i].GetBR() + m_CanVasLeftTop;
			ImVec2 displaySize = m_vecRect[i].GetSize();
			float texturewidth = (m_CurAtlas.Get()->GetWidth()) * 0.6f;
			float textureheight = (m_CurAtlas.Get()->GetHeight()) * 0.6f;

			ImVec2 uv0 = ImVec2(displayLT.x / texturewidth, displayLT.y / textureheight);
			ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x) / texturewidth, (displayLT.y + displaySize.y) / textureheight);

			//ImGui::Image((void*)tSRV.Get(), ImVec2(displaySize.x, displaySize.y),uv0,uv1 ,ImVec4(1,1,1,1), ImVec4(1, 1, 1, 1));
			ImGui::Image(m_CurAtlas.Get()->GetSRV().Get(), ImVec2(100.f, 100.f), uv0, uv1, ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
		}
	}


	ImGui::EndChild();
	ImGui::End();
}

void AnimationEditorUI::DrawCanvas()
{
	// canvas
	static ImVector<ImVec2> points;
	static ImVec2 scrolling(0.0f, 0.0f);
	static float sz = 1.f;
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

	m_CanVasLeftTop = canvas_p0;

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	//draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x * sz + scrolling.x, canvas_p0.y* sz + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	// MX = 마우스 포인트 위치 mouse_pos_in_canvas
// SX = element 와 화면 사이 거리
// SX2 = sz 처리 이후
// Scale = sz

// 새로운 위치
// SX2 = ( SX + MX) * ( Scale2 / Scale1) - mx
// SX2 = ( SX + mouse_pos_in_canvas) * ( Scale1 * sz / Scale1) - mouse_pos_in_canvas
// 
	// Canvas 안에 이미지출력
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	ComPtr<ID3D11ShaderResourceView> my_texture = NULL;

	m_CurAtlas = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");

	my_texture = m_CurAtlas.Get()->GetSRV().Get();
	float my_image_width = m_CurAtlas.Get()->GetWidth() * 0.6f;
	float my_image_height = m_CurAtlas.Get()->GetHeight() * 0.6f;

	ImVec2 left_top = ImVec2(scrolling.x, scrolling.y);
	ImVec2 right_bottom = ImVec2(my_image_width, my_image_height) * sz + ImVec2(scrolling.x, scrolling.y);

	
	draw_list->AddImage((void*)my_texture.Get(), left_top , right_bottom );
	draw_list->AddRect(left_top , right_bottom , IM_COL32(255, 255, 255, 255)); // 아틀라스테두리
	draw_list->PopClipRect();
	


	// 좌클릭시
	if (is_hovered && !cutting_rect && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		points.push_back(mouse_pos_in_canvas);
		points.push_back(mouse_pos_in_canvas);

		cutting_rect = true;
	}
	if (cutting_rect)
	{
		points.back() = mouse_pos_in_canvas;

		if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			cutting_rect = false;
			ImRect rect(points[points.size() - 2],points.back());
			m_vecRect.push_back(rect);
		}
	}

	// 오른쪽 마우스 드래그
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x * sz;
		scrolling.y += io.MouseDelta.y * sz;
	}

	// 마우스 휠
	if (io.MouseWheel > 0.f || io.MouseWheel)
	{
		sz += 0.1f * io.MouseWheel;
	}

	// 오른쪽 마우스 메뉴
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
		ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
	if (ImGui::BeginPopup("context"))
	{
		if (cutting_rect)
			points.resize(points.size() - 2);
		cutting_rect = false;
		if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) 
		{ 
			points.resize(points.size() - 2); 
		}
		if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) 
		{ 

			points.clear();
		}
		ImGui::EndPopup();
	}

	// Draw grid 
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f* sz;
		for (float x = fmodf(scrolling.x , GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	} 

	// draw rect
	for (int n = 0; n < points.Size; n += 2)
	{
		ImVec2 leftTop = ImVec2(origin.x + points[n].x, origin.y + points[n].y);
		ImVec2 rightBottom = ImVec2(ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y));
		if (leftTop.x > rightBottom.x)
		{
			leftTop.x = origin.x + points[n + 1].x;
			rightBottom.x = origin.x + points[n].x;
		}
		if (leftTop.y > rightBottom.y)
		{
			leftTop.y = origin.y + points[n + 1].y;
			rightBottom.y = origin.y + points[n].y;
		}
		draw_list->AddRect(leftTop ,rightBottom , IM_COL32(255, 255, 0, 255), 2.0f);
	}

	draw_list->PopClipRect();

}


void AnimationEditorUI::Deactivate()
{
	UI::Deactivate();
}


// MX = 마우스 포인트 위치 mouse_pos_in_canvas
// SX = element 와 화면 사이 거리
// SX2 = sz 처리 이후
// Scale = sz

// 새로운 위치
// SX2 = ( SX + MX) * ( Scale2 / Scale1) - mx
// SX2 = ( SX + mouse_pos_in_canvas) * ( Scale1 * sz / Scale1) - mouse_pos_in_canvas