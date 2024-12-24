#pragma once

#include <vector>
#include <string>
#include <algorithm>


enum DraggableItemCondiction : int {
	LEFT_COND = 0,
	RIGHT_COND = 1,
	TOP_COND = 2,
	BOT_COND = 3,
	CENTER_COND = 4,
	POOL_COND = 5,
	IN_MOVE_COND = 6,
};

struct MovableItems {
	std::string	ItemName;
	ImVec2 TemporaryPos = ImVec2();
	ImVec2 BasicPositions = ImVec2();
	ImVec2 WidgetSize = ImVec2();

	int Draw;
	int VectorCond = 4;

	int Type = 0; // 0 - text, 1 - bar, 2 - box

	MovableItems(
		int drw,
		std::string name = "Default",
		int cond = 4,
		ImVec2 temp1 = ImVec2{ },
		ImVec2 temp2 = ImVec2{ }, int type = 0)
	{
		Draw = drw;
		ItemName = name;
		VectorCond = cond;
		TemporaryPos = temp1;
		BasicPositions = temp2;
		Type = type;
	}
};
inline void Username(bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond)
{
	//ImVec2 ImTextSize = ImGui::CalcTextSize("Nickname");
	auto ImTextSize = pixel_s->CalcTextSizeA(13.f, FLT_MAX, NULL, ("Nickname"));
	//ImGui::PushFont(pixel_s);
	if (cond == LEFT_COND) {
		size = ImVec2(ImTextSize.x + 10, 14 + 6);
		draw->AddText(pos + ImVec2(1, size.y / 2 - ImTextSize.y / 2), ImColor(vars::visible::box_color), "Nickname");
	}

	if (cond == RIGHT_COND) {
		size = ImVec2(ImTextSize.x + 10, 14 + 6);
		draw->AddText(pos + ImVec2(9, size.y / 2 - ImTextSize.y / 2), ImColor(vars::visible::box_color), "Nickname");
	}

	if (cond == TOP_COND || cond == BOT_COND) {
		size = ImVec2(110, 14 + 6);
		draw->AddText(pos + ImVec2(55 - ImTextSize.x / 2, size.y / 2 - ImTextSize.y / 2), ImColor(vars::visible::box_color), "Nickname");
	}
	//ImGui::PopFont();
	if (cond == IN_MOVE_COND || cond == POOL_COND) {
		size = ImVec2(70, 20);

		draw->AddRectFilled(pos, pos + size, ImColor(50, 50, 50, 50));
		draw->AddRect(pos, pos + size, ImColor(0, 0, 0));
		//ImGui::PushFont(fMain);
		draw->AddText(pos + ImVec2(5, 3), ImColor(255, 255, 255), "Nickname");
		//ImGui::PopFont();
	}
}

inline void Weapon_(bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond)
{
	//ImVec2 ImTextSize = ImGui::CalcTextSize("Dual Elites");
	auto ImTextSize = pixel_s->CalcTextSizeA(13.f, FLT_MAX, NULL, ("Assault Rifle"));
	//ImGui::PushFont(pixel_s);
	if (cond == LEFT_COND) {
		size = ImVec2(ImTextSize.x + 10, ImTextSize.y + 6);
		draw->AddText(pos + ImVec2(1, 3), ImColor(vars::visible::box_color), "Assault Rifle");
	}

	if (cond == RIGHT_COND) {
		size = ImVec2(ImTextSize.x + 10, ImTextSize.y + 6);
		draw->AddText(pos + ImVec2(9, 3), ImColor(vars::visible::box_color), "Assault Rifle");
	}

	if (cond == TOP_COND || cond == BOT_COND) {
		size = ImVec2(110, ImTextSize.y + 6);
		draw->AddText(pos + ImVec2(55 - ImGui::CalcTextSize("Assault Rifle").x / 2, 3), ImColor(vars::visible::box_color), "Assault Rifle");
	}
	//ImGui::PopFont();
	if (cond == IN_MOVE_COND || cond == POOL_COND) {
		size = ImVec2(55, 20);


		draw->AddRectFilled(pos, pos + size, ImColor(50, 50, 50, 50));
		draw->AddRect(pos, pos + size, ImColor(0, 0, 0));
		//ImGui::PushFont(fMain);
		draw->AddText(pos + ImVec2(5, 3), ImColor(255, 255, 255), ("Weapon"));
		//ImGui::PopFont();
	}
}

inline void HealthBar(bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond)
{
	if (cond == LEFT_COND || cond == RIGHT_COND) {
		size = ImVec2(6, 230);
		draw->AddRectFilled(pos + ImVec2(4, 0), pos + size - ImVec2(4, 0), ImColor(vars::colors::Player_custom_healthColor));
		draw->AddRectFilled(pos + ImVec2(5, 1), pos + size - ImVec2(5, 1), ImColor(vars::colors::Player_custom_healthColor));

		draw->AddRectFilled(pos + ImVec2(5, 1), pos + size - ImVec2(5, 98), ImColor(vars::colors::Player_custom_healthColor));
	}

	if (cond == TOP_COND || cond == BOT_COND) {
		size = ImVec2(110, 10);
		draw->AddRectFilled(pos + ImVec2(4, 2), pos + size - ImVec2(4, 2), ImColor(vars::colors::Player_custom_healthColor));
		draw->AddRectFilled(pos + ImVec2(5, 3), pos + size - ImVec2(5, 3), ImColor(vars::colors::Player_custom_healthColor));

		draw->AddRectFilled(pos + ImVec2(5, 3), pos + size - ImVec2(56, 3), ImColor(vars::colors::Player_custom_healthColor));
	}

	if (cond == IN_MOVE_COND || cond == POOL_COND) {
		size = ImVec2(55, 20);

		draw->AddRectFilled(pos, pos + size, ImColor(50, 50, 50, 50));
		draw->AddRect(pos, pos + size, ImColor(0, 0, 0));
		//ImGui::PushFont(fMain);
		draw->AddText(pos + ImVec2(5, 3), ImColor(255, 255, 255), ("Health"));
		//ImGui::PopFont();
	}
}

inline void ArmorBar(bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond)
{
	if (cond == LEFT_COND || cond == RIGHT_COND) {
		size = ImVec2(6, 230);
		draw->AddRectFilled(pos + ImVec2(4, 0), pos + size - ImVec2(4, 0), ImColor(vars::visible::box_color));
		draw->AddRectFilled(pos + ImVec2(5, 1), pos + size - ImVec2(5, 1), ImColor(vars::visible::box_color));

		draw->AddRectFilled(pos + ImVec2(5, 1), pos + size - ImVec2(5, 98), ImColor(vars::visible::box_color));
	}

	if (cond == TOP_COND || cond == BOT_COND) {
		size = ImVec2(110, 10);
		draw->AddRectFilled(pos + ImVec2(4, 2), pos + size - ImVec2(4, 2), ImColor(vars::visible::box_color));
		draw->AddRectFilled(pos + ImVec2(5, 3), pos + size - ImVec2(5, 3), ImColor(vars::visible::box_color));

		draw->AddRectFilled(pos + ImVec2(5, 3), pos + size - ImVec2(56, 3), ImColor(vars::visible::box_color));
	}

	if (cond == IN_MOVE_COND || cond == POOL_COND) {
		size = ImVec2(45, 20);

		draw->AddRectFilled(pos, pos + size, ImColor(50, 50, 50, 50));
		draw->AddRect(pos, pos + size, ImColor(0, 0, 0));
		//ImGui::PushFont(fMain);
		draw->AddText(pos + ImVec2(5, 3), ImColor(255, 255, 255), ("Armor"));
		//ImGui::PopFont();
	}
}

inline void Box(bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond)
{
	if (cond == CENTER_COND) {
		size = ImVec2(110, 230);

		draw->AddRect(pos + ImVec2(4, 0), pos + size - ImVec2(4, 0), ImColor(vars::visible::box_color));
		draw->AddRect(pos + ImVec2(3, -1), pos + size - ImVec2(3, -1), ImColor(vars::visible::box_color));
		draw->AddRect(pos + ImVec2(5, 1), pos + size - ImVec2(5, 1), ImColor(vars::visible::box_color));
	}

	if (cond == IN_MOVE_COND || cond == POOL_COND) {
		size = ImVec2(35, 20);


		draw->AddRectFilled(pos, pos + size, ImColor(50, 50, 50, 50));
		draw->AddRect(pos, pos + size, ImColor(0, 0, 0));
		//ImGui::PushFont(fMain);
		draw->AddText(pos + ImVec2(5, 3), ImColor(255, 255, 255), ("Box"));
		//ImGui::PopFont();
	}
}

inline void GetDrawableEnemies(bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond, int var) {
	switch (var) {
	case 0: return Weapon_(pressed, draw, pos, size, cond); break;
	case 1: return Username(pressed, draw, pos, size, cond); break;
	case 2: return HealthBar(pressed, draw, pos, size, cond); break;
	//case 3: return ArmorBar(pressed, draw, pos, size, cond); break;
	case 3: return Box(pressed, draw, pos, size, cond); break;
	}
}

class c_esp_preview
{
public:
	c_esp_preview(int type, std::vector<MovableItems> items) : draw_type(type) {
		draggable_items[5].insert(draggable_items[5].end(), items.begin(), items.end());
	};

public:
	void instance()
	{
		for (auto a = 0; a < 7; a++)
		{
			for (auto b = 0; b < draggable_items[a].size(); b++)
			{
				if (handle(draggable_items[a][b]))
				{
					isMouseInAction = true;
					RecalculateAnimationFlag = true;

					if (mouse_intersect_rect(ImVec2(0, 440), ImVec2(330, 475))) {
						if (a == IN_MOVE_COND) {
							draggable_items[a][b].VectorCond = POOL_COND;
							vector_to_vector(draggable_items[a][b], POOL_COND);
						}
					}
					else if (mouse_intersect_rect(ImVec2(0, 120), ImVec2(110, 430 - 80)) && draggable_items[a][b].ItemName != ("Box")) {
						if (a == IN_MOVE_COND) {
							if (draggable_items[a][b].ItemName != ("Health") && draggable_items[a][b].ItemName != ("Armor")) {
								draggable_items[a][b].VectorCond = LEFT_COND;
								vector_to_vector(draggable_items[a][b], LEFT_COND);
							}
							else if (draggable_items[a][b].ItemName == ("Health")) {
								draggable_items[a][b].VectorCond = LEFT_COND;
								vector_to_vector_indexed(draggable_items[a][b], LEFT_COND, 0);
							}
							else if (draggable_items[a][b].ItemName == ("Armor")) {
								draggable_items[a][b].VectorCond = LEFT_COND;
								vector_to_vector_indexed(draggable_items[a][b], LEFT_COND, 0);
							}
						}
					}
					else if (mouse_intersect_rect(ImVec2(220, 120), ImVec2(330, 430 - 80)) && draggable_items[a][b].ItemName != ("Box")) {
						if (a == IN_MOVE_COND) {
							if (draggable_items[a][b].ItemName != ("Health") && draggable_items[a][b].ItemName != ("Armor")) {
								draggable_items[a][b].VectorCond = RIGHT_COND;
								vector_to_vector(draggable_items[a][b], RIGHT_COND);
							}
							else if (draggable_items[a][b].ItemName == ("Health")) {
								draggable_items[a][b].VectorCond = RIGHT_COND;
								vector_to_vector_indexed(draggable_items[a][b], RIGHT_COND, 0);
							}
							else if (draggable_items[a][b].ItemName == ("Armor")) {
								draggable_items[a][b].VectorCond = RIGHT_COND;
								vector_to_vector_indexed(draggable_items[a][b], RIGHT_COND, 0);
							}
						}
					}
					else if (mouse_intersect_rect(ImVec2(110, 35), ImVec2(220, 35 + 85)) && draggable_items[a][b].ItemName != ("Box")) {
						if (a == IN_MOVE_COND) {
							draggable_items[a][b].VectorCond = TOP_COND;
							vector_to_vector(draggable_items[a][b], TOP_COND);
						}
					}
					else if (mouse_intersect_rect(ImVec2(110, 430 - 80), ImVec2(220, 430)) && draggable_items[a][b].ItemName != ("Box")) {
						if (a == IN_MOVE_COND) {
							draggable_items[a][b].VectorCond = BOT_COND;
							vector_to_vector(draggable_items[a][b], BOT_COND);
						}
					}
					else if (mouse_intersect_rect(ImVec2(110, 120), ImVec2(220, 430 - 80)) && draggable_items[a][b].ItemName == ("Box")) {
						if (a == IN_MOVE_COND) {
							draggable_items[a][b].VectorCond = CENTER_COND;
							vector_to_vector(draggable_items[a][b], CENTER_COND);
						}
					}
					else {
						if (ImGui::GetIO().MouseDownDuration[0] > 0.0f) {
							vector_to_vector(draggable_items[a][b], IN_MOVE_COND);
						}
					}
				}

				if (!ImGui::GetIO().MouseDown[0])
					isMouseInAction = false;
			}
		}

		if (isMouseInAction == false)
		{
			if (RecalculateAnimationFlag == true) {
				RecalculateAnimation = 0.f;
				RecalculateAnimationFlag = false;
			}

			if (RecalculateAnimation < 1.f)
				RecalculateAnimation += 0.005f * (60 / ImGui::GetIO().Framerate);

			RecalculateAnimation = std::clamp(RecalculateAnimation, 0.f, 1.f);

			recalculate_pool(RecalculateAnimation);
			recalculate_sides(RecalculateAnimation);
			recalculate_top_bot(RecalculateAnimation);
		}
	}

	std::vector<std::vector<MovableItems>> draggable_items = {
		std::vector<MovableItems>{}, // Left 0 
		std::vector<MovableItems>{}, // Right 1
		std::vector<MovableItems>{}, // Top 2
		std::vector<MovableItems>{}, // Bot 3
		std::vector<MovableItems>{}, // Center 4
		std::vector<MovableItems>{}, // Pool 5
		std::vector<MovableItems>{}, // InMove 6
	};

private:
	bool item_in_move(MovableItems Item) {
		for (auto a = 0; a < draggable_items[IN_MOVE_COND].size(); a++) {
			if (draggable_items[IN_MOVE_COND][a].ItemName == Item.ItemName)
				return true;
		}
		return false;
	}


	void vector_to_vector(MovableItems Item, int Destination)
	{
		auto Position = get_movable_item_position(Item);

		draggable_items[std::get<0>(Position)].erase(draggable_items[std::get<0>(Position)].begin() + std::get<1>(Position)); // Erase From Position
		draggable_items[Destination].emplace_back(MovableItems(Item.Draw, Item.ItemName, Destination, Item.TemporaryPos, Item.BasicPositions, Item.Type)); // Append to Destination
	}

	void vector_to_vector_indexed(MovableItems Item, int Destination, int Index)
	{
		auto Position = get_movable_item_position(Item);

		draggable_items[std::get<0>(Position)].erase(draggable_items[std::get<0>(Position)].begin() + std::get<1>(Position)); // Erase From Position
		draggable_items[Destination].emplace(draggable_items[Destination].cbegin() + Index, MovableItems(Item.Draw, Item.ItemName, Destination, Item.TemporaryPos, Item.BasicPositions, Item.Type)); // Append to Destination
	}
	bool mouse_intersect_rect(ImVec2 pos1, ImVec2 pos2)
	{
		if (ImGui::GetIO().MousePos.x >= ImGui::GetWindowPos().x + pos1.x &&
			ImGui::GetIO().MousePos.y >= ImGui::GetWindowPos().y + pos1.y &&
			ImGui::GetIO().MousePos.x <= ImGui::GetWindowPos().x + pos2.x &&
			ImGui::GetIO().MousePos.y <= ImGui::GetWindowPos().y + pos2.y)
			return true;

		return false;
	}


	void recalculate_pool(float animation)
	{
		for (auto a = 0; a <= IN_MOVE_COND; a++)
		{
			for (auto b = 0; b < draggable_items[a].size(); b++)
			{
				if (a == IN_MOVE_COND || a == POOL_COND) {
					draggable_items[a][b].TemporaryPos = ImLerp(draggable_items[a][b].TemporaryPos, draggable_items[a][b].BasicPositions, animation);

					if (a == IN_MOVE_COND)
						vector_to_vector(draggable_items[a][b], POOL_COND);
				}
			}
		}
	}

	void recalculate_sides(float animation)
	{
		float PositionLeft = 0;
		float xModifier = 0;
		for (auto b = 0; b < draggable_items[LEFT_COND].size(); b++)
		{
			draggable_items[LEFT_COND][b].TemporaryPos = ImLerp(draggable_items[LEFT_COND][b].TemporaryPos, ImVec2(110 - draggable_items[LEFT_COND][b].WidgetSize.x - xModifier
				, 120 + PositionLeft), animation);

			if (b < 1)
			{
				if (draggable_items[LEFT_COND][0].ItemName == ("Health") || draggable_items[LEFT_COND][0].ItemName == ("Armor") ||
					draggable_items[LEFT_COND][1].ItemName == ("Health") || draggable_items[LEFT_COND][1].ItemName == ("Armor"))
					xModifier += 10;
			}

			if (draggable_items[LEFT_COND][b].ItemName != ("Health") && draggable_items[LEFT_COND][b].ItemName != ("Armor"))
				PositionLeft += draggable_items[LEFT_COND][b].WidgetSize.y;
		}


		float PositionRight = 0;
		float yModifier = 0;
		for (auto b = 0; b < draggable_items[RIGHT_COND].size(); b++)
		{
			draggable_items[RIGHT_COND][b].TemporaryPos = ImLerp(draggable_items[RIGHT_COND][b].TemporaryPos, ImVec2(220 + yModifier
				, 120 + PositionRight), animation);

			if (b < 1)
			{
				if (draggable_items[RIGHT_COND][0].ItemName == ("Health") || draggable_items[RIGHT_COND][0].ItemName == ("Armor") ||
					draggable_items[RIGHT_COND][1].ItemName == ("Health") || draggable_items[RIGHT_COND][1].ItemName == ("Armor"))
					yModifier += 10;
			}

			if (draggable_items[RIGHT_COND][b].ItemName != ("Health") && draggable_items[RIGHT_COND][b].ItemName != ("Armor"))
				PositionRight += draggable_items[RIGHT_COND][b].WidgetSize.y;
		}
	}

	void recalculate_top_bot(float animation)
	{
		float PositionTop = 0;
		for (auto b = 0; b < draggable_items[TOP_COND].size(); b++)
		{
			PositionTop += draggable_items[TOP_COND][b].WidgetSize.y;
			draggable_items[TOP_COND][b].TemporaryPos = ImLerp(draggable_items[TOP_COND][b].TemporaryPos, ImVec2(110, 35 + 85 - PositionTop), animation);
		}

		float PositionBot = 0;
		for (auto b = 0; b < draggable_items[BOT_COND].size(); b++)
		{
			draggable_items[BOT_COND][b].TemporaryPos = ImLerp(draggable_items[BOT_COND][b].TemporaryPos, ImVec2(110, 430 - 80 + PositionBot), animation);
			PositionBot += draggable_items[BOT_COND][b].WidgetSize.y;
		}

		for (auto b = 0; b < draggable_items[CENTER_COND].size(); b++)
		{
			draggable_items[CENTER_COND][b].TemporaryPos = ImLerp(draggable_items[CENTER_COND][b].TemporaryPos, ImVec2(110, 120), animation);
		}
	}


	std::tuple<int, int> get_movable_item_position(MovableItems Item)
	{
		for (auto a = 0; a < 7; a++) {
			for (auto b = 0; b < draggable_items[a].size(); b++) {
				if (draggable_items[a][b].ItemName == Item.ItemName)
					return std::make_tuple(a, b);
			}
		}
	}

	std::tuple<int, int> get_movable_item_position(std::string name)
	{
		for (auto a = 0; a < 7; a++) {
			for (auto b = 0; b < draggable_items[a].size(); b++) {
				if (draggable_items[a][b].ItemName == name)
					return std::make_tuple(a, b);
			}
		}
	}

	bool handle(MovableItems& Item)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiID id = window->GetID(Item.ItemName.c_str()); // use the pointer address as identifier
		ImGuiButtonFlags flags = 0;

		ImGuiContext& g = *GImGui;
		ImGuiStyle& style = g.Style;

		ImRect rect = ImRect(ImGui::GetWindowPos().x + Item.TemporaryPos.x,
			ImGui::GetWindowPos().y + Item.TemporaryPos.y,
			ImGui::GetWindowPos().x + Item.TemporaryPos.x + Item.WidgetSize.x,
			ImGui::GetWindowPos().y + Item.TemporaryPos.y + Item.WidgetSize.y);

		ImGui::ItemSize(rect, style.FramePadding.y);
		if (!ImGui::ItemAdd(rect, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, flags);
		bool is_active = ImGui::IsItemActive();

		ImGuiCol handle_color = ImColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

	
		GetDrawableEnemies(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(rect.Min, rect.Max), draw_list, ImGui::GetWindowPos() + Item.TemporaryPos, Item.WidgetSize, Item.VectorCond, Item.Draw);
		if (is_active && ImGui::IsMouseDragging(0)) {
			Item.TemporaryPos[0] += ImGui::GetIO().MouseDelta.x;
			Item.TemporaryPos[1] += ImGui::GetIO().MouseDelta.y;
		}

		return held;
	}

	int draw_type;

	bool isMouseInAction = false;
	float RecalculateAnimation = 0.f;
	bool RecalculateAnimationFlag = false;
};

inline c_esp_preview* player_esp_preview = new c_esp_preview(0, std::vector<MovableItems>{
	MovableItems(0, "Weapon", POOL_COND, { 10, 440 }, { 10, 440 }, 0),
		MovableItems(1, "Username", POOL_COND, { 70, 440 }, { 70, 440 }, 0),
		MovableItems(2, "Health", POOL_COND, { 150, 440 }, { 150, 440 }, 1),
	//	MovableItems(3, "Armor", POOL_COND, { 210, 440 }, { 210, 440 }, 1),
		MovableItems(3, "Box", POOL_COND, { 210, 440 }, { 260, 440 }, 1),
}
);

inline c_esp_preview* player_team_esp_preview = new c_esp_preview(1, std::vector<MovableItems>{
	MovableItems(0, "Weapon", POOL_COND, { 10, 440 }, { 10, 440 }, 0),
		MovableItems(1, "Username", POOL_COND, { 70, 440 }, { 70, 440 }, 0),
		MovableItems(2, "Health", POOL_COND, { 150, 440 }, { 150, 440 }, 1),
		MovableItems(3, "Armor", POOL_COND, { 210, 440 }, { 210, 440 }, 1),
		MovableItems(4, "Box", POOL_COND, { 260, 440 }, { 260, 440 }, 2),
}
);