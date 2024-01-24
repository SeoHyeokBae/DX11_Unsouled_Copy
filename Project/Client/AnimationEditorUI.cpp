#include "pch.h"
#include "AnimationEditorUI.h"

#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>

AnimationEditorUI::AnimationEditorUI()
	: UI("Animation Editor", "##AnimationEditor")
{
	Deactivate();



}

AnimationEditorUI::~AnimationEditorUI()
{
}

void AnimationEditorUI::render_update()
{
	static ImVector<ImVec2> points;
	static ImVec2 scrolling(0.0f, 0.0f);
	static bool opt_enable_grid = true;
	static bool adding_line = false;
	static bool opt_enable_context_menu = true;

	ImGui::Checkbox("Enable grid", &opt_enable_grid);
	ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);

	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);


	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	// 오른쪽 마우스 드래그
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// 오른쪽 마우스 메뉴
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
		ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
	if (ImGui::BeginPopup("context"))
	{
		if (adding_line)
			points.resize(points.size() - 2);
		adding_line = false;
		if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
		if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
		ImGui::EndPopup();
	}
	
	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	}
	for (int n = 0; n < points.Size; n += 2)
		draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
	draw_list->PopClipRect();

	// Canvas 안에 이미지출력
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	ComPtr<ID3D11ShaderResourceView> my_texture = NULL;
	Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");
	my_texture = pAltasTex.Get()->GetSRV().Get();
	float my_image_width = pAltasTex.Get()->GetWidth() * 0.6f;
	float my_image_height = pAltasTex.Get()->GetHeight() * 0.6f;

	ImVec2 left_top = ImVec2(50.f, 150.f) + ImVec2(scrolling.x, scrolling.y);
	ImVec2 right_bottom = ImVec2(my_image_width, my_image_height) + ImVec2(scrolling.x, scrolling.y);
	draw_list->AddImage((void*)my_texture.Get(), left_top, right_bottom);
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