#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
using std::vector;

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

const SDL_Rect drag_area = {0, 0, 640, 20};
static SDL_HitTestResult SDLCALL
hitTest(SDL_Window *window, const SDL_Point *pt, void *data)
{
    if (SDL_PointInRect(pt, &drag_area)) {
            return SDL_HITTEST_DRAGGABLE;
    }
    return SDL_HITTEST_NORMAL;
}





void draw_sequencer(vector<bool>& rset, int w, int h, uint colour)
{
    ImGui::BeginChild(4, ImVec2((w+3)*8+4,(w+3)*4), false);
    if(rset[0])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S00", ImVec2(w, h))) rset[0] = !rset[0];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S00", ImVec2(w, h))) rset[0] = !rset[0];
		ImGui::SameLine();
	}
	if(rset[1])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S01", ImVec2(w, h))) rset[1] = !rset[1];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S01", ImVec2(w, h))) rset[1] = !rset[1];
		ImGui::SameLine();
	}
	if(rset[2])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S02", ImVec2(w, h))) rset[2] = !rset[2];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S02", ImVec2(w, h))) rset[2] = !rset[2];
		ImGui::SameLine();
	}
	if(rset[3])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S03", ImVec2(w, h))) rset[3] = !rset[3];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S03", ImVec2(w, h))) rset[3] = !rset[3];
		ImGui::SameLine();
	}
	if(rset[4])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S04", ImVec2(w, h))) rset[4] = !rset[4];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S04", ImVec2(w, h))) rset[4] = !rset[4];
		ImGui::SameLine();
	}
	if(rset[5])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S05", ImVec2(w, h))) rset[5] = !rset[5];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S05", ImVec2(w, h))) rset[5] = !rset[5];
		ImGui::SameLine();
	}
	if(rset[6])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S06", ImVec2(w, h))) rset[6] = !rset[6];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S06", ImVec2(w, h))) rset[6] = !rset[6];
		ImGui::SameLine();
	}
	if(rset[7])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S07", ImVec2(w, h))) rset[7] = !rset[7];
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S07", ImVec2(w, h))) rset[7] = !rset[7];
	}
	if(rset[8])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S08", ImVec2(w, h))) rset[8] = !rset[8];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S08", ImVec2(w, h))) rset[8] = !rset[8];
		ImGui::SameLine();
	}
	if(rset[9])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S09", ImVec2(w, h))) rset[9] = !rset[9];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S09", ImVec2(w, h))) rset[9] = !rset[9];
		ImGui::SameLine();
	}
	if(rset[10])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S10", ImVec2(w, h))) rset[10] = !rset[10];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S10", ImVec2(w, h))) rset[10] = !rset[10];
		ImGui::SameLine();
	}
	if(rset[11])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S11", ImVec2(w, h))) rset[11] = !rset[11];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S11", ImVec2(w, h))) rset[11] = !rset[11];
		ImGui::SameLine();
	}
	if(rset[12])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S12", ImVec2(w, h))) rset[12] = !rset[12];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S12", ImVec2(w, h))) rset[12] = !rset[12];
		ImGui::SameLine();
	}
	if(rset[13])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S13", ImVec2(w, h))) rset[13] = !rset[13];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S13", ImVec2(w, h))) rset[13] = !rset[13];
		ImGui::SameLine();
	}
	if(rset[14])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S14", ImVec2(w, h))) rset[14] = !rset[14];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S14", ImVec2(w, h))) rset[14] = !rset[14];
		ImGui::SameLine();
	}
	if(rset[15])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S15", ImVec2(w, h))) rset[15] = !rset[15];
		ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S15", ImVec2(w, h))) rset[15] = !rset[15];
	}
	if(rset[16])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S16", ImVec2(w, h))) rset[16] = !rset[16];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S16", ImVec2(w, h))) rset[16] = !rset[16];
		ImGui::SameLine();
	}
	if(rset[17])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S17", ImVec2(w, h))) rset[17] = !rset[17];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S17", ImVec2(w, h))) rset[17] = !rset[17];
		ImGui::SameLine();
	}
	if(rset[18])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S18", ImVec2(w, h))) rset[18] = !rset[18];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S18", ImVec2(w, h))) rset[18] = !rset[18];
		ImGui::SameLine();
	}
	if(rset[19])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S19", ImVec2(w, h))) rset[19] = !rset[19];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S19", ImVec2(w, h))) rset[19] = !rset[19];
		ImGui::SameLine();
	}
	if(rset[20])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S20", ImVec2(w, h))) rset[20] = !rset[20];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S20", ImVec2(w, h))) rset[20] = !rset[20];
		ImGui::SameLine();
	}
	if(rset[21])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S21", ImVec2(w, h))) rset[21] = !rset[21];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S21", ImVec2(w, h))) rset[21] = !rset[21];
		ImGui::SameLine();
	}
	if(rset[22])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S22", ImVec2(w, h))) rset[22] = !rset[22];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S22", ImVec2(w, h))) rset[22] = !rset[22];
		ImGui::SameLine();
	}
	if(rset[23])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S23", ImVec2(w, h))) rset[23] = !rset[23];
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S23", ImVec2(w, h))) rset[23] = !rset[23];
	}
	if(rset[24])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S24", ImVec2(w, h))) rset[24] = !rset[24];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S24", ImVec2(w, h))) rset[24] = !rset[24];
		ImGui::SameLine();
	}
	if(rset[25])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S25", ImVec2(w, h))) rset[25] = !rset[25];
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S25", ImVec2(w, h))) rset[25] = !rset[25];
		ImGui::SameLine();
	}
	if(rset[26])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S26", ImVec2(w, h))) rset[26] = !rset[26];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S26", ImVec2(w, h))) rset[26] = !rset[26];
		ImGui::SameLine();
	}
	if(rset[27])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S27", ImVec2(w, h))) rset[27] = !rset[27];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S27", ImVec2(w, h))) rset[27] = !rset[27];
		ImGui::SameLine();
	}
	if(rset[28])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S28", ImVec2(w, h))) rset[28] = !rset[28];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S28", ImVec2(w, h))) rset[28] = !rset[28];
		ImGui::SameLine();
	}
	if(rset[29])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S29", ImVec2(w, h))) rset[29] = !rset[29];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S29", ImVec2(w, h))) rset[29] = !rset[29];
		ImGui::SameLine();
	}
	if(rset[30])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S30", ImVec2(w, h))) rset[30] = !rset[30];	
		ImGui::SameLine();
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S30", ImVec2(w, h))) rset[30] = !rset[30];
		ImGui::SameLine();
	}
	if(rset[31])
    {
        ImGui::PushStyleColor(ImGuiCol_Button, colour);
        if(ImGui::Button("##S31", ImVec2(w, h))) rset[31] = !rset[31];	
        ImGui::PopStyleColor();
    }
	else
	{
		if(ImGui::Button("##S31", ImVec2(w, h))) rset[31] = !rset[31];
	}
	
}

void set_style(void)
{
    ImGui::StyleColorsDark(0);
    ImGuiStyle * style = &ImGui::GetStyle();
    ImVec4 *cstyle = ImGui::GetStyle().Colors;

    //cstyle[ImGuiCol_FrameBg] = ImVec4(0.149, 0.196, 0.219,1);
	cstyle[ImGuiCol_FrameBg] = ImVec4(0.14,0.2,0.28,1);
    cstyle[ImGuiCol_Text] = ImVec4( 0.5, 0.7, 0.7, 1.0);
    style->WindowPadding = ImVec2(5, 5);
	style->WindowRounding = 2.0f;
	style->FramePadding = ImVec2(2, 2);
	style->FrameRounding = 2.0f;
	style->ItemSpacing = ImVec2(4, 4);
	style->ItemInnerSpacing = ImVec2(4, 4);
	style->IndentSpacing = 15.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 3.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 1.0f;
    style->PopupRounding = 3.0f;
}
