#include <vector>
#include <algorithm>
#include <windows.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
bool applied = false;
static bool sh_save_cfg = false;
static bool sh_load_cfg = false;
#include <chrono>
#include <xstring>

long getMils() {
	auto duration = std::chrono::system_clock::now().time_since_epoch();

	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

int y = 0;
#include <map>
#include "Elements/gui.h"
#include "Elements/colorpicker.h"
#include "../../../sk.h"
#include <random>


const char* const KeyNames[] = {
	"None",
	"Mouse1",
	"Mouse2",
	"Esc",
	"Mouse3",
	"Mouse4",
	"Mouse5",
	"None",
	"Back",
	"Tab",
	"None",
	"None",
	"Backspace",
	"Enter",
	"None",
	"None",
	"Shift",
	"Ctrl",
	"Alt",
	"Pause Break",
	"CapsLock",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"Esc",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"Print Screen",
	"Ins",
	"Del",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Left Windows",
	"Right Windows",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"Multiply",
	"+",
	"Separator",
	"Subtract",
	"-",
	"/",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Num Lock",
	"Scroll lock",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Left Shift",
	"Right Shift",
	"Left Ctrl",
	"Right Ctrl",
	"Left Alt",
	"Right Alt"
};


static bool _IsKeyPressedMap(ImGuiKey key, bool repeat = true)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed((ImGuiKey)key_index, repeat) : false;
}
inline bool     FocusableItemRegister(ImGuiWindow* window, ImGuiID id) { IM_ASSERT(0); IM_UNUSED(window); IM_UNUSED(id); return false; }
bool KeyBind(const char* label, int& k, const ImVec2& size_arg = ImVec2(0, 0))
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 1.0f);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	const bool focus_requested = FocusableItemRegister(window, g.ActiveId == id);
	//const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	//const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool hovered = ImGui::ItemHoverable(frame_bb, id);

	if (hovered) {
		ImGui::SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool user_clicked = hovered && io.MouseClicked[0];

	if (focus_requested || user_clicked) {
		if (g.ActiveId != id) {
			// Start edition
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));
			k = 0;
		}
		ImGui::SetActiveID(id, window);
		ImGui::FocusWindow(window);
	}
	else if (io.MouseClicked[0]) {
		// Release focus when we click outside
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	bool value_changed = false;
	int key = k;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io.MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
					break;
				}
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}
		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io.KeysDown[i]) {
					key = i;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
		}

		if (_IsKeyPressedMap(ImGuiKey_Escape)) {
			k = 0;
			ImGui::ClearActiveID();
		}
		else {
			k = key;
		}
	}

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

	char buf_display[64] = "None";

	ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, (ImColor(35, 35, 35)), true, 6.f);

	if (k != 0 && g.ActiveId != id) {
		strcpy_s(buf_display, KeyNames[k]);
	}
	else if (g.ActiveId == id) {
		strcpy_s(buf_display, "...");
	}

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding * 0.8, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
	//RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, GetColorU32(ImGuiCol_Text), style.ButtonTextAlign, &clip_rect);
	//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);

	if (label_size.x > 0)
		ImGui::RenderText(ImVec2(total_bb.Max.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

static class CStatus
{
public:
	bool	m_bIsLogged = false;
	bool	m_bIsLogging = false;
	bool	m_bIsInjecting = false;
	bool	m_bIsInjected = false;
} g_Status;

namespace menu {
	static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
	{
		const char* const* items = (const char* const*)data;
		if (out_text)
			*out_text = items[idx];
		return true;
	}

	bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
	{
		// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
		const char* items_separated_by_zeros = (const char*)data;
		int items_count = 0;
		const char* p = items_separated_by_zeros;
		while (*p)
		{
			if (idx == items_count)
				break;
			p += strlen(p) + 1;
			items_count++;
		}
		if (!*p)
			return false;
		if (out_text)
			*out_text = p;
		return true;
	}

	bool Checkbox(const char* label, bool* v)
	{
		auto window = ImGui::GetCurrentWindow();
		auto id = window->GetID(label);
		auto style = ImGui::GetStyle();

		float square_sz = 16;
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 label_size = ImGui::CalcTextSize(label, 0, 1);
		ImRect bb(pos, pos + ImVec2(ImGui::CalcItemWidth(), square_sz));

		ImGui::ItemSize(bb);
		ImGui::ItemAdd(bb, id);

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

		if (pressed) *v = !*v;

		float anim = ImTricks::Animations::FastFloatLerp(label, 0, hovered || *v, 0.f, 1.f, ui::anim_speed);
		float enabled_anim = ImTricks::Animations::FastFloatLerp(label, 1, *v, 0.f, 1.f, ui::anim_speed);
		float hovered_anim = ImTricks::Animations::FastFloatLerp(label, 2, hovered, 0.f, 1.f, ui::anim_speed);
		auto bg_col = ImTricks::Animations::FastColorLerp(ImGui::GetColorU32(ImGuiCol_FrameBg), ImGui::GetColorU32(ImGuiCol_Scheme), enabled_anim);
		auto dot_col = ImTricks::Animations::FastColorLerp(ImGui::GetColorU32(ImGuiCol_TextDisabled), ImGui::GetColorU32(ImGuiCol_Text), enabled_anim);
		auto text_col = ImTricks::Animations::FastColorLerp(ImGui::GetColorU32(ImGuiCol_TextDisabled), ImGui::GetColorU32(ImGuiCol_Text), anim);

		window->DrawList->AddRectFilled({ bb.Max.x - square_sz * 1.5f, bb.Min.y }, bb.Max, bg_col, square_sz / 2);
		window->DrawList->AddCircleFilled({ bb.Max.x - square_sz + square_sz / 2 * enabled_anim, bb.GetCenter().y }, 3 + hovered_anim, dot_col, 10);

		window->DrawList->AddText({ bb.Min.x, bb.GetCenter().y - label_size.y / 2 }, text_col, label, ImGui::FindRenderedTextEnd(label));

		/*if (col) {
			pos = window->DC.CursorPos;
			window->DC.CursorPos = ImVec2{ bb.Max.x - square_sz * 3 - style.ItemInnerSpacing.x, bb.Min.y };

			ImGui::BeginChild(std::string(label).append("cb_child").c_str(), { 24, square_sz }, 0, ImGuiWindowFlags_NoBackground);
			if (ui::color_btn(std::string(label).append("cb").c_str(), col, { 24, square_sz })) {
				ImGui::OpenPopup(std::string(label).append("popup").c_str());
			}

			ImGui::SetNextWindowSize({ 230, 0 });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 16, 16 });
			if (ImGui::BeginPopup(std::string(label).append("popup").c_str())) {
				color_picker(label, col);
				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
			ImGui::EndChild();

			window->DC.CursorPos = pos;
		}*/

		return pressed;
	}

	bool Slider(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
	{
		auto window = ImGui::GetCurrentWindow();
		auto id = window->GetID(label);
		auto style = ImGui::GetStyle();
		auto g = *GImGui;

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 label_size = ImGui::CalcTextSize(label, 0, 1);
		ImRect total_bb(pos, pos + ImVec2(ImGui::CalcItemWidth(), label_size.y + style.ItemInnerSpacing.y + 6));
		ImRect bb(total_bb.Max - ImVec2(ImGui::CalcItemWidth(), 6), total_bb.Max);

		ImGui::ItemSize(total_bb);
		ImGui::ItemAdd(total_bb, id);

		if (format == NULL)
			format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

		const bool hovered = ImGui::ItemHoverable(bb, id);

		const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
		const bool make_active = (clicked || g.NavActivateId == id || g.NavActivateInputId == id);
		//if (make_active && clicked)
		//	SetKeyOwner(ImGuiKey_MouseLeft, id);

		if (make_active) {
			ImGui::SetActiveID(id, window);
			ImGui::SetFocusID(id, window);
			ImGui::FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
		}

		ImRect grab_bb;
		const bool value_changed = ImGui::SliderBehavior(bb, id, data_type, p_data, p_min, p_max, format, NULL, &grab_bb);
		if (value_changed)
			ImGui::MarkItemEdited(id);

		char value_buf[64];
		const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

		auto anim = ImTricks::Animations::FastFloatLerp(label, 0, hovered || ImGui::IsItemActive(), 0, 1, ui::anim_speed);
		auto col = ImTricks::Animations::FastColorLerp(ImGui::GetColorU32(ImGuiCol_TextDisabled), ImGui::GetColorU32(ImGuiCol_Text), anim);

		static std::unordered_map< ImGuiID, float > values;
		auto value = values.find(id);

		if (value == values.end()) {
			values.insert({ id, 0.f });
			value = values.find(id);
		}

		value->second = ImLerp(value->second, grab_bb.Max.x - bb.Min.x, ui::anim_speed * 3.f);

		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), 2);
		window->DrawList->AddRectFilled(bb.Min, { bb.Min.x + value->second * ui::content_anim, bb.Max.y }, ImGui::GetColorU32(ImGuiCol_Scheme), 2);
		window->DrawList->AddCircleFilled({ bb.Min.x + value->second * ui::content_anim, bb.GetCenter().y }, 7, ImGui::GetColorU32(ImGuiCol_SliderGrab));

		//window->DrawList->AddText(total_bb.Min, col, label, ImGui::FindRenderedTextEnd(label));
		//window->DrawList->AddText({ total_bb.Max.x - ImGui::CalcTextSize(value_buf).x, total_bb.Min.y }, ImGui::GetColorU32(ImGuiCol_Text), value_buf);

		return value_changed;
	}

	bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		return Slider(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
	}

	static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
	{
		// This check is optional. Suppose we have two color widgets side by side, both widgets display different colors, but both colors have hue and/or saturation undefined.
		// With color check: hue/saturation is preserved in one widget. Editing color in one widget would reset hue/saturation in another one.
		// Without color check: common hue/saturation would be displayed in all widgets that have hue/saturation undefined.
		// g.ColorEditLastColor is stored as ImU32 RGB value: this essentially gives us color equality check with reduced precision.
		// Tiny external color changes would not be detected and this check would still pass. This is OK, since we only restore hue/saturation _only_ if they are undefined,
		// therefore this change flipping hue/saturation from undefined to a very tiny value would still be represented in color picker.
		ImGuiContext& g = *GImGui;
		if (g.ColorEditLastColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
			return;

		// When S == 0, H is undefined.
		// When H == 1 it wraps around to 0.
		if (*S == 0.0f || (*H == 0.0f && g.ColorEditLastHue == 1))
			*H = g.ColorEditLastHue;

		// When V == 0, S is undefined.
		if (*V == 0.0f)
			*S = g.ColorEditLastSat;
	}

	bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const float square_sz = ImGui::GetFrameHeight();
		const float w_full = ImGui::CalcItemWidth();
		const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
		const float w_inputs = w_full - w_button;
		const char* label_display_end = ImGui::FindRenderedTextEnd(label);
		g.NextItemData.ClearFlags();

		ImGui::BeginGroup();
		ImGui::PushID(label);

		// If we're not showing any slider there's no point in doing any HSV conversions
		const ImGuiColorEditFlags flags_untouched = flags;
		if (flags & ImGuiColorEditFlags_NoInputs)
			flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

		// Context menu: display and modify options (before defaults are applied)
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			ImGui::ColorEditOptionsPopup(col, flags);

		// Read stored options
		if (!(flags & ImGuiColorEditFlags_DisplayMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
		if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
		if (!(flags & ImGuiColorEditFlags_PickerMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
		if (!(flags & ImGuiColorEditFlags_InputMask_))
			flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
		flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

		const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
		const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
		const int components = alpha ? 4 : 3;

		// Convert to the formats we need
		float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
		if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
			ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
		{
			// Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
			ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
			ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
		}
		int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

		bool value_changed = false;
		bool value_changed_as_float = false;

		const ImVec2 pos = window->DC.CursorPos;
		const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
		window->DC.CursorPos.x = pos.x + inputs_offset_x;

		if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
		{
			// RGB/HSV 0..255 Sliders
			const float w_item_one = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
			const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

			const bool hide_prefix = (w_item_one <= ImGui::CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
			static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
			static const char* fmt_table_int[3][4] =
			{
				{   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
				{ "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
				{ "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
			};
			static const char* fmt_table_float[3][4] =
			{
				{   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
				{ "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
				{ "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
			};
			const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

			for (int n = 0; n < components; n++)
			{
				if (n > 0)
					ImGui::SameLine(0, style.ItemInnerSpacing.x);
				ImGui::SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

				// FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
				if (flags & ImGuiColorEditFlags_Float)
				{
					value_changed |= ImGui::DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
					value_changed_as_float |= value_changed;
				}
				else
				{
					value_changed |= ImGui::DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
				}
				if (!(flags & ImGuiColorEditFlags_NoOptions))
					ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
			}
		}
		else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
		{
			// RGB Hexadecimal Input
			char buf[64];
			if (alpha)
				ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
			else
				ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
			ImGui::SetNextItemWidth(w_inputs);
			if (ImGui::InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
			{
				value_changed = true;
				char* p = buf;
				while (*p == '#' || ImCharIsBlankA(*p))
					p++;
				i[0] = i[1] = i[2] = 0;
				i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
				int r;
				if (alpha)
					r = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
				else
					r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
				IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
			}
			if (!(flags & ImGuiColorEditFlags_NoOptions))
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		}

		ImGuiWindow* picker_active_window = NULL;
		if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
		{
			const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
			window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

			const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
			if (ImGui::ColorButton("##ColorButton", col_v4, flags | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, { 18, 18 }))
			{
				if (!(flags & ImGuiColorEditFlags_NoPicker))
				{
					// Store current color and open a picker
					g.ColorPickerRef = col_v4;
					ImGui::OpenPopup("picker");
					ImGui::SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
				}
			}
			if (!(flags & ImGuiColorEditFlags_NoOptions))
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

			ImGui::SetNextWindowSize({ 230, 0 });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 16, 16 });
			if (ImGui::BeginPopup("picker"))
			{
				if (g.CurrentWindow->BeginCount == 1)
				{
					picker_active_window = g.CurrentWindow;
					value_changed |= color_picker(label, col);
				}
				ImGui::EndPopup();
			}
			ImGui::PopStyleVar();
		}

		ImGui::RenderText({ pos.x + 18 + style.ItemInnerSpacing.x, pos.y + 9 - GImGui->FontSize / 2 + 1 }, label);

		// Convert back
		if (value_changed && picker_active_window == NULL)
		{
			if (!value_changed_as_float)
				for (int n = 0; n < 4; n++)
					f[n] = i[n] / 255.0f;
			if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
			{
				g.ColorEditLastHue = f[0];
				g.ColorEditLastSat = f[1];
				ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
				g.ColorEditLastColor = ImGui::ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
			}
			if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
				ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha)
				col[3] = f[3];
		}

		ImGui::PopID();
		ImGui::EndGroup();

		// Drag and Drop Target
		// NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
		if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && ImGui::BeginDragDropTarget())
		{
			bool accepted_drag_drop = false;
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
			{
				memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512 //-V1086
				value_changed = accepted_drag_drop = true;
			}
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
			{
				memcpy((float*)col, payload->Data, sizeof(float) * components);
				value_changed = accepted_drag_drop = true;
			}

			// Drag-drop payloads are always RGB
			if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
				ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
			ImGui::EndDragDropTarget();
		}

		// When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
		if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
			g.LastItemData.ID = g.ActiveId;

		if (value_changed && g.LastItemData.ID != 0) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
			ImGui::MarkItemEdited(g.LastItemData.ID);

		return value_changed;
	}

	IMGUI_API bool			InputText(const char* label, const char* icon, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);

	bool InputText(const char* label, const char* icon, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		ImVec2 pos = window->DC.CursorPos;
		const float width = ImGui::CalcItemSize(ImVec2(-0.1f, 0.f), 0.f, 0.f).x;

		window->DrawList->AddRectFilled(pos + ImVec2(width - ImGui::GetFrameHeight(), ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y), pos + ImVec2(width, ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y + ImGui::GetFrameHeight()), ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameRounding, ImDrawFlags_RoundCornersRight);
		window->DrawList->AddRectFilled(pos + ImVec2(0, ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y), pos + ImVec2(width - ImGui::GetFrameHeight(), ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y + ImGui::GetFrameHeight()), ImGui::GetColorU32(ImGuiCol_FrameBg), g.Style.FrameRounding, ImDrawFlags_RoundCornersLeft);

		// Renders
		if (strlen(icon))
		{
			ImVec2 icon_size = ImGui::CalcTextSize(icon);
			window->DrawList->AddText(pos + ImVec2(width - ImGui::GetFrameHeight() + (ImGui::GetFrameHeight() / 2 - icon_size.x / 2) + 1, ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y + (ImGui::GetFrameHeight() / 2 - icon_size.y / 2) + 1), ImGui::GetColorU32(ImGuiCol_WindowBg), icon);
		}

		// Input Widget
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(g.Style.ItemSpacing.x, g.Style.ItemInnerSpacing.y));
		ImGui::TextDisabled(label);
		IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()
		ImGui::PushItemWidth(width - ImGui::GetFrameHeight());
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		auto result = ImGui::InputTextEx(std::string("##Input_" + std::string(label)).c_str(), NULL, buf, (int)buf_size, ImVec2(0, 0), flags, callback, user_data);
		ImGui::PopStyleColor();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		return result;
	}
}
namespace menu
{
	static const char* healthbar_style[]{
		"None",
		"Left Side",
		"Bottom",
	};
	static const char* healthbar_color_style[]{
	"Defualt",
	"Gradient",
	};
	static const char* healthbar_color[]{
"Defualt",
"Custom",
	};
	static const char* npc_healthbar_color_style[]{
"Defualt",
"Gradient",
	};
	static const char* chams_type[]{
"Glow",
"Wire Frame",
"Set Trought",
"Lit Chams",
"Standart",
"Hologram",
"jopa",
"jopa 2",
"jopa 3",
"jopa 4",
"jopa 5",
"jopa 6",
"jopa 7",
"jopa 8",
"jopa 9",
	};
	static const char* npc_healthbar_color[]{
"Defualt",
"Custom",
	};
	static const char* npc_healthbar_style[]{
		"None",
		"Left Side",
		"Bottom",
	};
	static const char* building_grade[]{
		"Twig",
		"Wood",
		"Stone",
		"Metal",
		"HQM",
	};
	static const char* npc_box_style[]{
		"None",
		"Default",
		"Cornered",
	};
	static const char* box_style[]{
		"None",
		"Default",
		"Cornered",
		"3D Box",
	};
	static const char* anti_aim_yaw[]{
		"None",
		"Backwards",
		"Backwards (down)",
		"Backwards (up)",
		"Left",
		"Left (down)",
		"Left (up)",
		"Right",
		"Right (down)",
		"Right (up)",
		"Jitter",
		"Jitter (down)",
		"Jitter (up)",
		"Spin",
		"Spin (down)",
		"Spin (up)",
		"Random",
	};
	static const char* fake_lag_type[]{
		"Basic",
		"Double Tap",
	};
	static const char* target_tracer_pos[]{
		"Default",
		"Middle",
	};
	static const char* tracer_pos[]{
		"Default",
		"Middle",
	};
	static const char* modelstate_pos[]{
		"Default",
		"On Ladder",
		"On Ground",
		"Ducked",
		"Sleeping",
	};
	static const char* autoshoot_type[]{
		"Silent",
		"Default",
	};
	static const char* desync_autoshoot_type[]{
		"Always",
		"Desync",
	};
	static const char* timeshoot_type[]{
		"Silent",
		"Default",
	};
	static const char* ignore[]{
		"Ignore NPC",
		"Ignore Teammate",
		"Ignore Sleeping",
	};
	static const char* hitmaterial[]{
	"Default",
	"Glass",
	"Water",
	"Wood",
	"Metal",
	"Sand",
	"Grass",
	"Rock",
	"Concrete",
	"Forest",
	"Cloth",
	"Generic",
	"Null",
	};
	static const char* hitsound_type[]{
    "NL",
	};
	static const char* name_pos[]{
    "Bottom",
    "Top",
	};
	static const char* off[]{
	"Bottom",
	"Top",
	};
	static bool selected[3];
	static std::string previewValue1 = "";
	static int pTabs = 0;
	int pSubTabs = 0;
	int pSubTabs2 = 0;
	int pSubTabs3 = 0;
	int pSubTabs4 = 0;
	static int pSubTabs1 = 0;
	static int pWeapons = 0;
	static bool toggled;



	static bool menu_added = false;

	struct Configs
	{
		std::string name;
		std::string fullname;
		bool loaded;
	};
	std::vector<Configs> configs;
	std::vector<Configs> luas;

	static bool login_once = false;

	void DrawMenu()
	{
		auto& s = ImGui::GetStyle();
		auto config11 = ImFontConfig();
		config11.FontDataOwnedByAtlas = false;

		ImGuiIO& io = ImGui::GetIO();

		ui::styles();
		ui::colors();

		ui::add_page(0, []() { // aimbot

			ImGui::BeginGroup();
			{
				ui::begin_child("Target");
				{
					menu::Checkbox("Perfect Silent", &vars::combat::psilent);
					
					if (vars::combat::psilent) {
						menu::Checkbox("Draw Targeting Fov", &vars::combat::visualize_fov);
						if (vars::combat::visualize_fov) {
							ImGui::SliderInt("Targeting Fov", &vars::combat::fov, 0, 5000, "%d%%");
						}
					}
					ImGui::Combo("Target Hitbox", &vars::combat::onal, "Head\0Body\0");
				}
				struct InfoBoxData {
					const char* icon;
					const char* label;
					ImVec2 size;
				};

			

			
				ui::end_child();

				ui::begin_child("Exploits");
				{
					menu::Checkbox("Shoot Throught Wall", &vars::combat::stw);
					menu::Checkbox("Model Prank", &vars::misc::model_mem);
					menu::Checkbox("Instahit", &vars::combat::instanthit);
					menu::Checkbox("test", &vars::visuals::GameSense);	
					menu::Checkbox("Lov Velocity", &vars::combat::lv);
					if (vars::combat::lv) {
						KeyBind(("Lov Velocity Bind"), vars::keys::lv_key, ImVec2(80, 35));
					}

					menu::Checkbox("Anti Aim", &vars::misc::anti_aim);
					if (vars::misc::anti_aim) {
					ImGui::Combo(("Anti Aim Type"), &vars::misc::anti_aim_yaw, anti_aim_yaw, IM_ARRAYSIZE(anti_aim_yaw));
					}
					menu::Checkbox("Hit Sound", &vars::misc::custom_hitsound);
					if (vars::misc::custom_hitsound) {
						ImGui::Combo("Hit Sound Type", &vars::misc::hitsound_rejim, "NL\0");
					}
					menu::Checkbox("Bullet Tp", &vars::combat::bullet_tp);
					if (vars::combat::bullet_tp) {
					
						menu::Checkbox("Scan", &vars::combat::hitscan);
					

						ImGui::Combo("Sphere Type", &vars::visuals::ponan_type, "None\0Sphere\0Gizmo\0Box");
					}
					menu::Checkbox("Bullet Teleport On Helicopter", &vars::combat::always_heli_rotor);
					menu::Checkbox("teleport", &vars::combat::teleport);
					if (vars::combat::teleport) {
						KeyBind(("Teleport"), vars::keys::teleport, ImVec2(80, 35));
					}
					menu::Checkbox("Manipulator", &vars::combat::manipulator);
					
					if (vars::combat::manipulator) {
						ImGui::Combo(("Mode"), &vars::combat::ishak1212, "Dynamic\0Static\0");
						menu::Checkbox("Auto Stop", &vars::misc::Mautostop);
						if (vars::misc::Mautostop) {
							KeyBind(("Stop Bind"), vars::keys::autostopperm, ImVec2(80, 35));
						}
						menu::Checkbox("Auto Shoot", &vars::combat::autoshoot);
						if (vars::combat::autoshoot) {
							ImGui::Combo("Shoot Type", &vars::combat::desync_autoshoot_type, "Always\0Manipulator\0");
						}
						KeyBind(("Manipulator Bind"), vars::keys::manipulated_key, ImVec2(80, 35));
						
						menu::Checkbox("Farm bot", &vars::misc::farmbot);
							KeyBind(("Fly Bind"), vars::keys::paste, ImVec2(80, 35));

						
					}
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			{
				ui::begin_child("Ignores");
				{
					menu::Checkbox("Ignore NPC", &vars::combat::ignore_npc);
					menu::Checkbox("Ignore Sleepers", &vars::combat::ignore_sleepers);
					menu::Checkbox("Ignore Teammates", &vars::combat::ignore_team);
				}
				ui::end_child();

				ui::begin_child("Other");
				{
					menu::Checkbox("Shoot Throught Material", &vars::weapons::penetrate);
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			});

		ui::add_page(1, []() { // visuals

			ImGui::BeginGroup();
			{
				ui::begin_child("Players");
				{
					ImGui::Combo("Player Box", &vars::players::boxstyle, "None\0Default\0Cornered\0");
					menu::Checkbox("Show Target Name", &vars::players::name);
					if (vars::players::name)
					{
						ImGui::Combo(("Name Position"), &vars::players::namePos, name_pos, IM_ARRAYSIZE(name_pos));
					}
					menu::Checkbox("OOF ARROWS", &vars::players::oof_arrows);
					if (vars::players::oof_arrows)
					{
						ImGui::Combo(("Style"), &vars::players::Outstyle, off, IM_ARRAYSIZE(off));
					}
					menu::Checkbox("Show Target Health", &vars::players::healthbar);
					menu::Checkbox("Show Target Weapon", &vars::players::weapon);
					if (vars::players::weapon)
					{
						ImGui::Combo(("Weapon Position"), &vars::players::weaponPos, name_pos, IM_ARRAYSIZE(name_pos));
					}
					menu::Checkbox("Show Target Skeleton", &vars::players::skeleton);
					menu::Checkbox("Target Line", &vars::players::targetline);
					if (vars::players::targetline) {
						ImGui::Combo("Position", &vars::players::targetline_pos, "Default\0Middle\0");
					}
					menu::Checkbox(("Dont Show Sleepers"), &vars::players::sleeperignore);
				}
				ui::end_child();

				ui::begin_child("Main");
				{
					ImGui::Combo("Hit Effect", &vars::misc::hitmaterial, "Default\0Glass\0");
					ImGui::Text("Game Fov");
					ImGui::SliderInt("Game Fov", &vars::misc::fov, 30, 140, "%d%%");
					menu::Checkbox("Zoom", &vars::visuals::zoom);
					if (vars::visuals::zoom) {
						KeyBind(("Zoom Bind"), vars::keys::zoom, ImVec2(80, 35));
					}
					menu::Checkbox("Flyhack Indicator", &vars::misc::flyhack_indicator);
		
					menu::Checkbox("Manipulator Indicator", &vars::combat::instakill_indicator);

					menu::Checkbox("Manipulator Indicator", &vars::misc::reload_coldown);
					
					menu::Checkbox("Name Incicator", &vars::visuals::target_name);
					menu::Checkbox("Hit Logs", &vars::misc::hit_logs);
					menu::Checkbox("Bullet Tracer", &vars::misc::bullet_tracers);
					menu::Checkbox("Hit Marker", &vars::misc::hitmarker);
					menu::Checkbox("Full Ambient Bright", &vars::misc::bright_ambient);
					menu::Checkbox("Custom Time", &vars::misc::custom_time);
					if (vars::misc::custom_time) {
						ImGui::SliderInt("Time", &vars::misc::time, 0, 12, "%d%%");
					}
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			{
				ui::begin_child("Colors");
				{
					menu::ColorEdit4(("Skeleton Color##Color"), vars::visible::skeleton_color, ImGuiColorEditFlags_NoInputs);
					menu::ColorEdit4(("Esp Color##Color"), vars::visible::box_color, ImGuiColorEditFlags_NoInputs);
					menu::ColorEdit4(("Teammate Color##Color"), vars::colors::team_color, ImGuiColorEditFlags_NoInputs);
					menu::ColorEdit4(("Sleepers##Color"), vars::colors::sleep_color, ImGuiColorEditFlags_NoInputs);
				}
				ui::end_child();

				ui::begin_child("NPC");
				{
					ImGui::Combo("NPC Box", &vars::npc::boxstyle, "None\0Default\0Cornered\0");
					menu::Checkbox("Show NPC Name", &vars::npc::name);
					menu::Checkbox("Show NPC Distance", &vars::npc::distance);
					menu::Checkbox("Show NPC Health", &vars::npc::healthbar);
					menu::Checkbox("Show NPC Weapon", &vars::npc::weapon);
					menu::Checkbox("Show NPC Skeleton", &vars::npc::skeleton);
				}
				ui::end_child();

				ui::begin_child("Crosshairs");
				{
					menu::Checkbox("Cadicall Crosshair", &vars::crosshair::cadicall);
					menu::Checkbox("Dor Crosshair", &vars::crosshair::dot);
				}
				ui::end_child();

				ui::begin_child("Hand Chams");
				{
					menu::Checkbox(("Hand Chams"), &vars::visuals::hand_chams);
					if (vars::visuals::hand_chams) {
						menu::ColorEdit4((""), (float*)&vars::colors::hand_chams, ImGuiColorEditFlags_NoInputs);
						ImGui::Combo(("Style"), &vars::visuals::hand_chams_type, chams_type, IM_ARRAYSIZE(chams_type));
					}
				}
				ui::end_child();

				ui::begin_child("World ESP");
				{
					menu::Checkbox(("Dropped Item"), &vars::visuals::other::dropped_items);
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			});

#define ICON_FA_KEY "\xef\x82\x84"	// U+f084

		ui::add_page(2, []() { // weapons

			ImGui::BeginGroup();
			{
				ui::begin_child("Weapon Customize");
				{
					ImGui::Text("Recoil");
					ImGui::SliderInt("Recoil", &vars::weapons::recoil_control, 0, 100, "%d%%");
					menu::Checkbox("No Spread", &vars::weapons::no_spread);
					menu::Checkbox("No Shotgun Spread", &vars::weapons::no_sway);
					menu::Checkbox("Faster Bullet", &vars::weapons::fast_bullets);
					menu::Checkbox("Automatic", &vars::weapons::automatic);
					menu::Checkbox("Shoot From Minicopter", &vars::weapons::minicopter_aim);
					menu::Checkbox("No Shoot Restrictions", &vars::misc::can_attack);
					if (vars::players::targetline) {
						ImGui::Combo("Position", &vars::players::boxstyle, "Default\0Middle\0");
					}
					menu::Checkbox(("Dont Show Sleepers"), &vars::players::sleeperignore);
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			{
				ui::begin_child("Other");
				{
					menu::Checkbox("Instant Eoka", &vars::weapons::eokatap);
					menu::Checkbox("Skip Bow Shoot", &vars::weapons::fastbow);
					menu::Checkbox("Fast Shoot", &vars::weapons::rapid_fire);
					menu::Checkbox("Silent Reload", &vars::combat::autoreload);
					menu::Checkbox("No Attack Animation", &vars::weapons::remove_attack_anim);
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			});

		ui::add_page(3, []() { // Misc

			ImGui::BeginGroup();
			{
				ui::begin_child("Movement");
				{
					menu::Checkbox("No Collision", &vars::misc::walker);
					menu::Checkbox("Omnidirectional Sprint", &vars::misc::omnidirectional_sprinting);
					menu::Checkbox("Climb Wall", &vars::misc::spiderman);
					menu::Checkbox("No Jump Restrictions", &vars::misc::inf_jump);
					menu::Checkbox("Shoot From Minicopter", &vars::weapons::minicopter_aim);
					menu::Checkbox("No Shoot Restrictions", &vars::misc::can_attack);
					menu::Checkbox(("No Fall Damage"), &vars::misc::no_fall);
					menu::Checkbox(("Fly Stopper"), &vars::misc::fly_auto_stopper);
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			{
				ui::begin_child("Game");
				{
					menu::Checkbox("Instant Loot", &vars::misc::fast_loot);
					menu::Checkbox("Suicide", &vars::misc::mass_suicide);
					menu::Checkbox("Weapon Spam", &vars::misc::weapon_spam);
					if (vars::misc::weapon_spam) {
						KeyBind(("Spam Bind"), vars::keys::weaponspam, ImVec2(80, 35));
					}
					menu::Checkbox("Hammer Spam On Player", &vars::combat::hammerspam);
					if (vars::combat::hammerspam) {
						ImGui::Combo(("Material"), &vars::combat::ishak, "Glass\0Water\0");
					}
					menu::Checkbox("Hammer Spam On Self", &vars::combat::hammerspam1);
					if (vars::combat::hammerspam1) {
						ImGui::Combo(("Material"), &vars::combat::ishak, "Glass\0Water\0");
					}
					menu::Checkbox("Silent Melee", &vars::combat::silent_melee);
					menu::Checkbox("Silent Healing", &vars::misc::faster_healing);
					menu::Checkbox("Admin Flags", &vars::misc::fakeadmin);
					menu::Checkbox("Speed Hack", &vars::misc::speedhack);
					if (vars::misc::speedhack) {
						KeyBind(("SpeedHack Bind"), vars::keys::speedkey, ImVec2(80, 35));
					}
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			});

		ui::add_page(4, []() { // Configs

			ImGui::BeginGroup();
			{
				ui::begin_child("Configs");
				{
					ImGui::InputText("", vars::stuff::ConfigName, 0x100);
					if (ImGui::Button2(("Save"), ImVec2(0, 0))) {
						config.Save();
						notify::add_log("Logs", "Config Saved", ImColor(accent_color[0], accent_color[1], accent_color[2]), ImColor(255, 255, 255), 4700);
						sh_save_cfg = true;
					}
					ImGui::SameLine();
					if (ImGui::Button2(("Load"), ImVec2(0, 0))) {
						config.Load();
						notify::add_log("Logs", "Config Loaded", ImColor(accent_color[0], accent_color[1], accent_color[2]), ImColor(255, 255, 255), 4700);
						sh_load_cfg = true;
					}
					if (ImGui::Button2(("Open Folder"), ImVec2(0, 0))) {
						system(("start C:\\pasta_naxyi"));
					}
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			{
				ui::begin_child("Other");
				{
					if (ImGui::Button2(("Unload Cheat"), ImVec2(0, 0))) {
						vars::stuff::Panic = true;
					}
				}
				ui::end_child();
			}
			ImGui::EndGroup();

			});

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("UI", 0, ImGuiWindowFlags_NoDecoration);
		{
			ImGui::PopStyleVar();
			ImGui::SetWindowSize(ui::size);

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, GImGui->Style.WindowRounding);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
			ImGui::BeginChild("header", { -1, 60 });
			{
				for (int i = 0; i < ui::tabs.size(); ++i) {

					ui::tab(i);

					ImGui::SameLine(0, 0);
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar(2);

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ui::content_anim);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 20, 20 });
			ImGui::BeginChild("content", { -1, -1 }, 0, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoBackground);
			{
				ImGui::PopStyleVar();

				ui::render_page();
			}
			ImGui::EndChild();

			ImGui::PopStyleVar();
		}
		ImGui::End();

		ui::handle_alpha_anim();

		ImGui::Render();
	}
}


