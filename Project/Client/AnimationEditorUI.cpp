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
	// todo

	DrawCanvas();

	ImGui::Begin("list" , &m_bOpen);
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImVec2(250.f,250.f);   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	ImGui::End();

	//float my_image_width = m_CurAtlas.Get()->GetWidth() / canvas_sz.x;
	//float my_image_height = m_CurAtlas.Get()->GetHeight() / canvas_sz.y;
	//ImGui::Image((void*)m_CurAtlas.Get(), ImVec2(my_image_width, my_image_height));

	// Sprite 나열
	ImGui::Begin("Sprite", &m_bOpen);
	ImGui::BeginChild("child", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
	if (nullptr != m_CurAtlas)
	{
		for (int i = 0; i < m_vecRect.size(); i++)
		{
			if ( 0 != i)
				ImGui::SameLine(100.f * i);

			ImVec2 displayLT = m_vecRect[i].GetTL() + m_CanVasLeftTop;
			ImVec2 displayRB = m_vecRect[i].GetBR() + m_CanVasLeftTop;
			ImVec2 displaySize = m_vecRect[i].GetSize();
			float texturewidth = (m_CurAtlas.Get()->GetWidth()) ;
			float textureheight = (m_CurAtlas.Get()->GetHeight()) ;
			ComPtr<ID3D11ShaderResourceView> tSRV = m_CurAtlas.Get()->GetSRV();

			ImVec2 uv0 = ImVec2(displayLT.x / texturewidth, displayLT.y / textureheight);
			ImVec2 uv1 = ImVec2((displayLT.x + displaySize.x) / texturewidth, (displayLT.y + displaySize.y) / textureheight);

			ImGui::Image((void*)tSRV.Get(), ImVec2(displaySize.x, displaySize.y),uv0,uv1 ,ImVec4(1,1,1,1), ImVec4(1, 1, 1, 1));

			//ImGui::Text("size = %d x %d", my_image_width, my_image_height); 24 17

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
	ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);

	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail() ;   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	m_CanVasLeftTop = canvas_p0;

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	//draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));


	// Canvas 안에 이미지출력
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	ComPtr<ID3D11ShaderResourceView> my_texture = NULL;

	m_CurAtlas = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");

	my_texture = m_CurAtlas.Get()->GetSRV().Get();
	float my_image_width = m_CurAtlas.Get()->GetWidth();
	float my_image_height = m_CurAtlas.Get()->GetHeight();

	ImVec2 left_top = ImVec2(scrolling.x, scrolling.y);
	ImVec2 right_bottom = ImVec2(my_image_width, my_image_height) + ImVec2(scrolling.x, scrolling.y);
	
	draw_list->AddImage((void*)my_texture.Get(), left_top * sz, right_bottom * sz);
	draw_list->AddRect(left_top * sz, right_bottom * sz, IM_COL32(255, 255, 255, 255)); // 아틀라스테두리
	draw_list->PopClipRect();
	
	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);


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
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// 마우스 휠
	if (io.MouseWheel > 0.f || io.MouseWheel)
	{
		sz += 0.1f*io.MouseWheel;
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
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
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
		draw_list->AddRect(leftTop,rightBottom, IM_COL32(255, 255, 0, 255), 2.0f);	
	}

	draw_list->PopClipRect();

}


void AnimationEditorUI::Deactivate()
{
	UI::Deactivate();
}

//ImGui::Begin("DirectX11 Texture Test");
//ImGui::End();


	////텍스처출력
	////ID3D11ShaderResourceView* my_texture = NULL;
	//ComPtr<ID3D11ShaderResourceView> my_texture = NULL;
	//Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");
	//my_texture = pAltasTex.Get()->GetSRV().Get();
	//float my_image_width = pAltasTex.Get()->GetWidth() * 0.3f;
	//float my_image_height = pAltasTex.Get()->GetHeight() * 0.3f;
	//
	//ImGui::Image((void*)my_texture.Get(), ImVec2(my_image_width, my_image_height));
	//ImGui::Text("pointer = %p", my_texture);
	//ImGui::Text("size = %d x %d", my_image_width, my_image_height);