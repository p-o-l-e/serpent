#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
using std::vector;

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

const 	SDL_Rect drag_area = {0, 0, 640, 20};
static 	SDL_HitTestResult SDLCALL
hitTest(SDL_Window *window, const SDL_Point *pt, void *data)
{
    if (SDL_PointInRect(pt, &drag_area)) {
            return SDL_HITTEST_DRAGGABLE;
    }
    return SDL_HITTEST_NORMAL;
}



const char* const sq_labels[] = {	"##S00", "##S01", "##S02", "##S03", "##S04", "##S05", "##S06", "##S07",
									"##S08", "##S09", "##S10", "##S11", "##S12", "##S13", "##S14", "##S15",
									"##S16", "##S17", "##S18", "##S19", "##S20", "##S21", "##S22", "##S23",
									"##S24", "##S25", "##S26", "##S27", "##S28", "##S29", "##S30", "##S31" };

void draw_sequencer(vector<bool>& rset, int w, int h, uint colour)
{
    ImGui::BeginChild(4, ImVec2((w+3)*8+4,(w+3)*4), false);

	for(int i=0; i<32; i++)
	{
		if(rset[i])
		{
			ImGui::PushStyleColor(ImGuiCol_Button, colour);
			if(ImGui::Button(sq_labels[i], ImVec2(w, h))) rset[i] = !rset[i];
			ImGui::PopStyleColor();
		}
		else
		{
			if(ImGui::Button(sq_labels[i], ImVec2(w, h))) rset[i] = !rset[i];
		}
		if((i+1)%8) ImGui::SameLine();
	}
	ImGui::EndChild();
}

void set_style(void)
{
    ImGui::StyleColorsDark(0);
    ImGuiStyle * style = &ImGui::GetStyle();
    ImVec4 *cstyle = ImGui::GetStyle().Colors;

	cstyle[ImGuiCol_FrameBg] 	= ImVec4(0.14f, 0.20f, 0.28f, 1.00f);
    cstyle[ImGuiCol_Text] 		= ImVec4(0.50f, 0.70f, 0.70f, 1.00f);
    style->WindowPadding 		= ImVec2(5, 5);
	style->WindowRounding 		= 2.00f;
	style->FramePadding 		= ImVec2(2, 2);
	style->FrameRounding 		= 2.00f;
	style->ItemSpacing 			= ImVec2(4, 4);
	style->ItemInnerSpacing 	= ImVec2(4, 4);
	style->IndentSpacing 		= 15.0f;
	style->ScrollbarSize 		= 15.0f;
	style->ScrollbarRounding 	= 3.00f;
	style->GrabMinSize 			= 5.00f;
	style->GrabRounding 		= 1.00f;
    style->PopupRounding 		= 3.00f;
}

struct constraints
{
	ImVec4 blue_base;
	ImVec4 blue_indigo;
	ImVec4 blue_glow;
	ImVec4 blue_teal;
	ImVec4 red_coral;
	ImVec4 red_salmon;

	float scale = 1.00f;
	ImVec2 window_main;
	ImVec2 child_a;
	ImVec2 child_osc;
	float  slider_a;
	void set();
};

void constraints::set()
{
	window_main.x 	= 788.0f * scale;
	window_main.y	= 510.0f * scale;

	child_a.x 		= 470.0f * scale;
	child_a.y 		= 460.0f * scale;

	child_osc.x 	= 154.0f * scale;
	child_osc.y 	= 206.0f * scale;

	slider_a		= 132.0f * scale;

	blue_base 		= ImVec4(0.140f, 0.200f, 0.280f, 1.000f);
	blue_glow		= ImVec4(0.239f, 0.521f, 0.878f, 1.000f);
	blue_indigo 	= ImVec4(0.129f, 0.215f, 0.349f, 1.000f);
	blue_teal		= ImVec4(0.149f, 0.251f, 0.353f, 1.000f);
	red_coral		= ImVec4(0.706f, 0.314f, 0.314f, 1.000f); // 0xB45050FF 
	red_salmon		= ImVec4(0.659f, 0.396f, 0.317f, 1.000f); // 0xA86551FF
	// IM_COL32(61, 133, 224, 255) 0.239f
	// IM_COL32(33, 55, 89, 255)
	//(0.129f, )
	// IM_COL32(38, 64, 90,255) = (0.149f, 0.251f, 0.353f, 1.000f)
	// IM_COL32(180, 80, 80, 255)
	
}

constraints cs;