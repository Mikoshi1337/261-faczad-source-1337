struct m_indicator
{
	std::string m_text;
	ImColor m_color;
	m_indicator(const char* text, ImColor color) :
		m_text(text), m_color(color)
	{

	}
	m_indicator(std::string text, ImColor color) :
		m_text(text), m_color(color)
	{

	}
};
std::vector<m_indicator> m_indicators;
void AddIndicators() {
	float width = vars::stuff::ScreenWidth, height = vars::stuff::ScreenHeight;
	int h = 0;
	for (auto& indicator : m_indicators) {
		auto size_text = indShandow->CalcTextSizeA(30.f, FLT_MAX, NULL, indicator.m_text.c_str());
		g_Render->FilledRectGradient(14, height - 340 - h - 3, size_text.x / 2, 33, ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 165), ImColor(0, 0, 0, 165), ImColor(0, 0, 0, 0));
		g_Render->FilledRectGradient(14 + size_text.x / 2, height - 340 - h - 3, size_text.x / 2, 33, ImColor(0, 0, 0, 165), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 165));
		//	g_Render->DrawString(21 + 1, height - 340 - h + 1, ImColor(0, 0, 0, 200), 0, indShandow, 30.f, indicator.m_text.c_str());
		g_Render->DrawString(21, height - 340 - h, indicator.m_color, render2::outline, indShandow, 30.f, indicator.m_text.c_str());
		h += 36;
	}
	m_indicators.clear();
}
struct m_keybind
{
	ImDrawList* drawList;
	ImVec2 position;
	const char* m_text;
	int bind_key;
	//Color m_color;
	m_keybind(ImDrawList* drawList, ImVec2 position, const char* text, int bind) : drawList(drawList), position(position), m_text(text), bind_key(bind) {
	}
};
std::vector<m_keybind> m_keybinds;
void Addbind() {
	auto offset = 2;
	for (auto& indicator : m_keybinds) {
		if (GetAsyncKeyState(indicator.bind_key)) {
			indicator.drawList->AddText(flogs, 13.f, ImVec2(indicator.position.x + 7, indicator.position.y + 25 + 4 + offset), ImColor(255, 255, 255), indicator.m_text);
			indicator.drawList->AddText(flogs, 13.f, ImVec2(indicator.position.x + 107, indicator.position.y + 25 + 4 + offset), ImColor(128, 128, 128), ("on"));
			offset += 15.f;
		}
	}
	m_keybinds.clear();
}
void keybind() {
	ImVec2 position;
	ImDrawList* drawList_test;
	auto count = 0;
	if (vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key))  count++;
	if (vars::combat::instakill && GetAsyncKeyState(vars::keys::instakill)) count++;
	if (vars::misc::long_neck && GetAsyncKeyState(vars::keys::longneck)) count++;
	if (vars::combat::desync && GetAsyncKeyState(vars::keys::desync)) count++;
	if (vars::visuals::zoom && GetAsyncKeyState(vars::keys::zoom)) count++;
	if (vars::misc::interactive_debug && GetAsyncKeyState(vars::keys::debugging)) count++;
	if (vars::misc::speedhack && GetAsyncKeyState(vars::keys::speedkey)) count++;
	if (vars::misc::weapon_spam && GetAsyncKeyState(vars::keys::weaponspam)) count++;
	ImGui::SetNextWindowSize(ImVec2(125, 25 * count + 25));
	ImGui::Begin(("keybinds"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize); {
		auto offset = 2;
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList_test = drawList;
		const ImVec2 origin(ImGui::GetCursorScreenPos());
		position = ImGui::GetWindowPos();
		drawList->AddRectFilled(position, ImVec2(position.x + 125, position.y + 25), ImColor(0, 0, 0, 222));//167
		drawList->AddRectFilled(position, ImVec2(position.x + 125, position.y + 2), ImColor(accent_color[0], accent_color[1], accent_color[2]));//167
		auto size_text = flogs->CalcTextSizeA(13.f, FLT_MAX, NULL, ("Keybinds"));
		drawList->AddText(flogs, 13.f, ImVec2(position.x + 125 / 2 - size_text.x / 2, position.y + 25 / 2 - size_text.y / 2), ImColor(255, 255, 255), ("Keybinds"));
		m_keybinds.push_back(m_keybind(drawList, position, "Manipulator", vars::keys::manipulated_key));
		m_keybinds.push_back(m_keybind(drawList, position, "Timeshot", vars::keys::instakill));
		m_keybinds.push_back(m_keybind(drawList, position, "Long Neck", vars::keys::longneck));
		m_keybinds.push_back(m_keybind(drawList, position, "Desync", vars::keys::desync));
		m_keybinds.push_back(m_keybind(drawList, position, "Zoom", vars::keys::zoom));
		m_keybinds.push_back(m_keybind(drawList, position, "Debug", vars::keys::debugging));
		m_keybinds.push_back(m_keybind(drawList, position, "Time Scale", vars::keys::speedkey));
		m_keybinds.push_back(m_keybind(drawList, position, "Fake Shoots", vars::keys::weaponspam));
	}
	ImGui::End();
	drawRectangleShadowVerticesAdaptive(drawList_test, position + ImVec2(5, 1), ImVec2(115, 24), ImColor(accent_color[0], accent_color[1], accent_color[2], accent_color[3]), ImVec2(0, 0), shadowSettings);
}

//void RenderWatermark()
//{
//	ImVec4* colors = ImGui::GetStyle().Colors;
//	colors[ImGuiCol_Text] = ImColor(255, 255, 255);
//
//	float sWidth = vars::stuff::ScreenWidth;
//	ImGuiStyle* style = &ImGui::GetStyle();
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1, 1));
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.000f, 0.286f, 0.655f, 0.000f));
//	const char* _text;
//	_text = ("harmony.connect");
//	float uhodvlevaSpeed = 1.00000;
//	static float levas = 0;
//	levas -= uhodvlevaSpeed;
//	if (levas < -100.f) levas = sWidth + 100;
//	for (int i = 10; i < 690; i++)
//	{
//		float hueg = levas + (1.f / (float)1200) * i;
//		if (hueg < 0.f) hueg += 1.0f;
//		ImGui::SetNextWindowPos(ImVec2(hueg - ImGui::CalcTextSize(_text).x / 2, 10));
//	}
//	ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize(_text).x + 10, 20));
//	ImGui::Begin(("Watermark"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoInputs);
//	{
//		//ImGui::PushFont(calibri_);
//
//		ImGui::SetCursorPos(ImVec2(5, 5));
//		ImGui::Text(_text);
//
//		//ImGui::PopFont();
//	}
//	ImGui::PopStyleColor();
//	ImGui::PopStyleVar(3);
//}

void pre_draw() {
	//static std::map< ImGuiID, TabAnimation > TabMap;
	ImGuiContext& g = *GImGui;
	if (vars::crosshair::swastika) {
		float xs = vars::stuff::ScreenWidth / 2, ys = vars::stuff::ScreenHeight / 2;
		static float rotation_degree = 0.f;
		if (rotation_degree > 89.f)
			rotation_degree = 0.f;
		rotation_degree += vars::crosshair::speed; // скорость вращения свастона
		int length = (int)(vars::stuff::ScreenWidth / vars::crosshair::razmer / 15); // размер свастона
		float gamma = atan(length / length);
		int i = 0;
		while (i < 4)
		{
			std::vector <int> p
			{
				int(length * sin(Math::GRD_TO_BOG(rotation_degree + (i * 90)))),
				int(length * cos(Math::GRD_TO_BOG(rotation_degree + (i * 90)))),
				int((length / cos(gamma)) * sin(Math::GRD_TO_BOG(rotation_degree + (i * 90) + RAD2DEG(gamma)))),
				int((length / cos(gamma)) * cos(Math::GRD_TO_BOG(rotation_degree + (i * 90) + RAD2DEG(gamma))))
			};
			g_Render->DrawLine(xs, ys, xs + p[0], ys - p[1], ImColor(vars::colors::swaston_color), 1.f);
			g_Render->DrawLine(xs + p[0], ys - p[1], xs + p[2], ys - p[3], ImColor(vars::colors::swaston_color), 1.f);
			i++;
		}
	}
	auto* TargetPlayerA = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
	if (vars::visuals::GameSense) {
		if (GetAsyncKeyState(vars::keys::manipulated_key)) m_indicators.push_back({ "MP", ImColor(0, 255, 0,255) });
		if (GetAsyncKeyState(vars::keys::instakill)) m_indicators.push_back({ "TM", ImColor(255, 255, 255,255) });
		if (GetAsyncKeyState(vars::keys::longneck)) m_indicators.push_back({ "LN", ImColor(255, 255, 255,255) });
		if (GetAsyncKeyState(vars::keys::desync)) m_indicators.push_back({ "Desync", ImColor(255, 255, 255,255) });
		if (GetAsyncKeyState(vars::keys::zoom)) m_indicators.push_back({ "Zoom", ImColor(255, 255, 255,255) });
		if (GetAsyncKeyState(vars::keys::debugging)) m_indicators.push_back({ "Debug", ImColor(255, 255, 255,255) });
		if (GetAsyncKeyState(vars::keys::speedkey)) m_indicators.push_back({ "TM", ImColor(255, 255, 255,255) });
		if (GetAsyncKeyState(vars::keys::weaponspam)) m_indicators.push_back({ "FS", ImColor(255, 255, 255,255) });
	}
	if (vars::visuals::keybinds) keybind();
	AddIndicators();
	Addbind();
	//RenderWatermark();
	float xs = vars::stuff::ScreenWidth / 2, ys = vars::stuff::ScreenHeight / 2;
	if (vars::misc::flyhack_indicator) {
		if (vars::stuff::flyhack / vars::stuff::max_flyhack >= 3.f) {
			g_Render->Progbar({ xs - 150, ys - 490 }, { 300, 7 }, vars::stuff::flyhack / vars::stuff::max_flyhack, vars::stuff::flyhack / vars::stuff::max_flyhack);
		}
		else {
			g_Render->Progbar({ xs - 150, ys - 500 }, { 300, 7 }, vars::stuff::flyhack / vars::stuff::max_flyhack, 1.f);
		}

		if (vars::stuff::hor_flyhack / vars::stuff::max_hor_flyhack >= 6.5f) {
			g_Render->Progbar({ xs - 150, ys - 490 }, { 300, 7 }, vars::stuff::hor_flyhack / vars::stuff::max_hor_flyhack, vars::stuff::hor_flyhack / vars::stuff::max_hor_flyhack);
		}
		else {
			g_Render->Progbar({ xs - 150, ys - 490 }, { 300, 7 }, vars::stuff::hor_flyhack / vars::stuff::max_hor_flyhack, 1.f);
		}
	}

	if (vars::crosshair::default) {
		g_Render->DrawLine(xs, ys, xs + 4, ys + 4, ImColor(255, 255, 255, 255), 0.7f);
		g_Render->DrawLine(xs, ys, xs + 4, ys - 4, ImColor(255, 255, 255, 255), 0.7f);
		g_Render->DrawLine(xs, ys, xs - 4, ys - 4, ImColor(255, 255, 255, 255), 0.7f);
		g_Render->DrawLine(xs, ys, xs - 4, ys + 4, ImColor(255, 255, 255, 255), 0.7f);

		g_Render->DrawLine(xs + 4, ys + 4, xs + 4 + 4, ys + 4 + 4, ImColor(vars::colors::default_color), 0.7f);
		g_Render->DrawLine(xs + 4, ys - 4, xs + 4 + 4, ys - 4 - 4, ImColor(vars::colors::default_color), 0.7f);
		g_Render->DrawLine(xs - 4, ys - 4, xs - 4 - 4, ys - 4 - 4, ImColor(vars::colors::default_color), 0.7f);
		g_Render->DrawLine(xs - 4, ys + 4, xs - 4 - 4, ys + 4 + 4, ImColor(vars::colors::default_color), 0.7f);
	}
	if (GetAsyncKeyState(vars::keys::lv_key)) {
		g_Render->DrawString(xs, ys - 150, ImColor(255, 0, 0), render2::outline | render2::centered_x, calibri_, 16, ("-Low Velocity Predicted-"));
	}

	if (vars::crosshair::custom) {
		g_Render->FilledRectGradient(xs + vars::crosshair::gap, ys, vars::crosshair::length, 1, ImColor(vars::colors::custom_color), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f), ImColor(vars::colors::custom_color));
		g_Render->FilledRectGradient(xs - vars::crosshair::gap + 2, ys, -vars::crosshair::length, 1, ImColor(vars::colors::custom_color), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f), ImColor(vars::colors::custom_color));
		g_Render->FilledRectGradient(xs, ys - vars::crosshair::gap + 2, 1, -vars::crosshair::length, ImColor(vars::colors::custom_color), ImColor(vars::colors::custom_color), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f));
		g_Render->FilledRectGradient(xs, ys + vars::crosshair::gap, 1, vars::crosshair::length, ImColor(vars::colors::custom_color), ImColor(vars::colors::custom_color), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f), ImColor(vars::colors::custom_color[0], vars::colors::custom_color[1], vars::colors::custom_color[2], 0.0f));
	}
	if (vars::crosshair::dot) {
		g_Render->CircleFilled(xs, ys + 1.f, 1.8, ImColor(vars::colors::dot_color), 0);
	}
	if (vars::crosshair::cadicall) {
		g_Render->FilledRect(xs + 8.f, ys, 8.f, 3, ImColor(vars::colors::cadicall_color), 0, 0);
		g_Render->FilledRect(xs - 8.f, ys, -8.f, 3, ImColor(vars::colors::cadicall_color), 0, 0);
		g_Render->CircleFilled(xs, ys + 1.f, 1.8, ImColor(vars::colors::cadicall_color), 0);
	}
	if (vars::combat::visualize_fov) {
		g_Render->DrawCircle(xs, ys, vars::combat::fov, 1.2f, ImColor(vars::colors::fov_color));
		//	g_Render->AddRadialGradient(ImVec2(xs, ys), 10.f, ImColor(vars::colors::fov_color), ImColor(0, 0, 0, 0));
	}
	float FOV = vars::combat::fov, CurFOV;
	if (vars::stor::closestPlayer != NULL) {
		if (FOV < (CurFOV = GetFov(TargetPlayerA, BoneList(0.5))) && !vars::combat::lock_target) {
			vars::stor::closestPlayer = NULL;
		}
	}
	if (vars::stor::closestHeli != NULL) {
		Vector3 pos = reinterpret_cast<BaseEntity*>(vars::stor::closestHeli)->transform()->position();
		if (FOV < (CurFOV = GetFovHeli(pos))) {
			vars::stor::closestHeli = NULL;
		}
	}

	if (reinterpret_cast<BaseEntity*>(vars::stor::closestPlayer)->IsDestroyed()) {
		vars::stor::closestPlayer = NULL;
	}
	if (reinterpret_cast<BaseEntity*>(vars::stor::closestHeli)->IsDestroyed()) {
		vars::stor::closestHeli = NULL;
	}
	if (vars::combat::lock_target && vars::stor::closestPlayer != NULL) {
		//	OxyRender::Text(Vector2{xs, ys - 70}, D2D1::ColorF::LightBlue, true, true, (L"%s locked"), TargetPlayerA->_displayName( ));
	}
	if (vars::combat::manipulator && !other::m_manipulate.empty() && vars::stor::closestPlayer != NULL) {
		std::string string;
		char buf[256] = { 0 }; sprintf_s(buf, (u8"-manipulated-"));
		string += buf;
		g_Render->DrawString(xs, ys - -20, ImColor(255, 0, 0), render2::outline | render2::centered_x, calibri_, 12, string.c_str());
	}

	//if (vars::combat::pastapon) {
	//	if (LocalPlayer::Entity() == ModelStateFlag::Flying) {
	//}

	if (vars::visuals::target_name) {
		if (vars::stor::closestPlayer != NULL) {
			std::string string;
			Vector3 LocalPos = LocalPlayer::Entity()->transform_(head)->position();
			char buf[256] = { 0 }; sprintf_s(buf, ("target: %ls | %dm"), TargetPlayerA->_displayName(), (int)LocalPos.distance(target_player->get_bone_pos(l_foot)));
			string += buf;
		
			g_Render->DrawString(xs, ys - -10, ImColor(255, 255, 255,255), render2::outline | render2::centered_x, calibri_, 12, string.c_str());
		}
	}

	if (vars::ores::closest_ore) {
		static float screenX = vars::stuff::ScreenWidth;
		static float screenY = vars::stuff::ScreenHeight;
		Vector2 startPos = Vector2(screenX / 2.f, screenY);

		f_object tr1 = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head), ("ore.prefab"));
		Vector2 screen;
		if (utils::w2s(tr1.position, screen) && tr1.valid) {
			g_Render->DrawLine(startPos.x, startPos.y, screen.x, screen.y, ImColor(255, 255, 255), 1.f);

			f_object tr2 = f_object::get_closest_object(tr1.position, ("ore.prefab"), tr1.position);
			Vector2 screen2;
			if (utils::w2s(tr2.position, screen2) && tr2.valid) {
				g_Render->DrawLine(screen.x, screen.y, screen2.x, screen2.y, ImColor(255, 255, 255), 1.f);

				f_object tr3 = f_object::get_closest_object(tr2.position, ("ore.prefab"), tr1.position, tr2.position);
				Vector2 screen3;
				if (utils::w2s(tr3.position, screen3) && tr3.valid) {
					g_Render->DrawLine(screen2.x, screen2.y, screen3.x, screen3.y, ImColor(255, 255, 255), 1.f);

					f_object tr4 = f_object::get_closest_object(tr3.position, ("ore.prefab"), tr1.position, tr2.position, tr3.position);
					Vector2 screen4;
					if (utils::w2s(tr4.position, screen4) && tr4.valid) {
						g_Render->DrawLine(screen3.x, screen3.y, screen4.x, screen4.y, ImColor(255, 255, 255), 1.f);
					}
				}
			}
		}
	}
	if (vars::players::targetline) {
		if (vars::stor::closestPlayer != NULL && TargetPlayerA->IsValid()) {
			static float screenX = vars::stuff::ScreenWidth;
			static float screenY = vars::stuff::ScreenHeight;
			static Vector2 startPos;
			if (vars::players::targetline_pos == 0) {
				startPos = Vector2(screenX / 2.f, screenY);
			}
			if (vars::players::targetline_pos == 1) {
				startPos = Vector2(screenX / 2, screenY / 2);
			}
			Vector2 ScreenPos;
			if (!(TargetPlayerA->get_bone_pos(spine1).x == 0 && TargetPlayerA->get_bone_pos(spine1).y == 0 && TargetPlayerA->get_bone_pos(spine1).z == 0)) {
				if (utils::w2s(TargetPlayerA->get_bone_pos(head), ScreenPos)) {
					g_Render->DrawLine(startPos.x, startPos.y, ScreenPos.x, ScreenPos.y, ImColor(255, 255, 255), 1.f);
				}
			}
		}
	}
	if (vars::combat::visualize_prediction && TargetPlayerA->IsValid()) {
		Vector2 t;
		if (!(TargetPlayerA->get_bone_pos(spine1).x == 0 && TargetPlayerA->get_bone_pos(spine1).y == 0 && TargetPlayerA->get_bone_pos(spine1).z == 0)) {
			if (utils::w2s(a::get_aim_point(GetBulletSpeed(), GetGravity(LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo())), t)) {
				g_Render->DrawString(t.x, t.y, ImColor(vars::colors::prediction_color), render2::outline | render2::centered_x, pixel_s, 11, ("X"));

			}
		}
	}
	if (vars::stor::closestHeli != NULL && vars::visuals::patrol_heli) {
		static float screenX = vars::stuff::ScreenWidth;
		static float screenY = vars::stuff::ScreenHeight;
		static Vector2 startPos = Vector2(screenX / 2.f, screenY - 200.f);
		Vector2 ScreenPos;
		if ((int)ceil(read(vars::stor::closestHeli + 0x20C, float)) > 0) {
			if (utils::w2s(reinterpret_cast<BaseEntity*>(vars::stor::closestHeli)->transform()->position(), ScreenPos));
			//Renderer::Line(startPos, ScreenPos, D2D1::ColorF(0.3f, 0.34f, 1.f), 1.f);
		}

	}
	if (vars::combat::smooth_factor <= 0) { vars::combat::smooth_factor = 1; }
	if (vars::misc::grade_ > 4) { vars::misc::grade_ = 4; }
	if (vars::weapons::hitdistance < 300) { vars::weapons::hitdistance = 300; }
	//if (TargetPlayerA) {
	//	if (vars::stor::closestPlayer != NULL && !TargetPlayerA->IsNpc() && vars::players::belt && TargetPlayerA->IsValid()) {
	//		const float Height = 140.f;
	//		const float Width = 160.f;
	//		POINT p;
	//		if (GetCursorPos(&p)) {
	//			if (p.x >= vars::players::beltx && p.x <= vars::players::beltx + Width) {
	//				if (p.y >= vars::players::belty && p.y <= vars::players::belty + Height) {
	//					if (GetAsyncKeyState(VK_LBUTTON) && show) {
	//						vars::players::beltx = p.x - (Width / 2);
	//						vars::players::belty = p.y - (Height / 2);
	//					}
	//				}
	//			}
	//		}
	//		OxyRender::FillRectangle({ vars::players::beltx, vars::players::belty }, { Width, Height }, D2D1::ColorF(0.f, 0.f, 0.f, 0.47f));
	//		OxyRender::Text({ vars::players::beltx + (Width / 2), vars::players::belty + 10 }, D2D1::ColorF::White, true, true, (L"Hotbar %s"), TargetPlayerA->_displayName());
	//		OxyRender::Line({ vars::players::beltx, vars::players::belty + 20 }, { vars::players::beltx + Width, vars::players::belty + 20 }, D2D1::ColorF(0.71f, 0.27f, 0.49f), 1);
	//		float Pos = 0;
	//		if (TargetPlayerA->item_list_b()) {
	//			for (int i = 0; i < TargetPlayerA->item_list_b()->get_size(); i++) { // belt
	//				Item* ActWeapon = TargetPlayerA->item_list_b()->get_value(i);
	//				if (ActWeapon) {
	//					if (ActWeapon == TargetPlayerA->GetActiveWeapon()) {
	//						wchar_t* ActiveItem = ActWeapon->get_name();
	//						if (ActiveItem) {
	//							OxyRender::Text({ vars::players::beltx + (Width / 2), vars::players::belty + 40 + Pos }, D2D1::ColorF::Red, true, true, (L"%s"), ActiveItem);
	//						}
	//					}
	//					else {
	//						wchar_t* ActiveItem = ActWeapon->get_name();
	//						if (ActiveItem) {
	//							OxyRender::Text({ vars::players::beltx + (Width / 2), vars::players::belty + 40 + Pos }, D2D1::ColorF::White, true, true, (L"%s"), ActiveItem);
	//						}
	//					}
	//				}
	//				Pos += 15;
	//			}
	//		}
	//	}
	//}
	//if (TargetPlayerA) {
	//	if (vars::stor::closestPlayer != NULL && !TargetPlayerA->IsNpc() && vars::players::clothes && TargetPlayerA->IsValid()) {
	//		const float Height = 150.f;
	//		const float Width = 160.f;
	//		POINT p;
	//		if (GetCursorPos(&p)) {
	//			if (p.x >= vars::players::clothesx && p.x <= vars::players::clothesx + Width) {
	//				if (p.y >= vars::players::clothesy && p.y <= vars::players::clothesy + Height) {
	//					if (GetAsyncKeyState(VK_LBUTTON) && show) {
	//						vars::players::clothesx = p.x - (Width / 2);
	//						vars::players::clothesy = p.y - (Height / 2);
	//					}
	//				}
	//			}
	//		}
	//		OxyRender::FillRectangle({ vars::players::clothesx, vars::players::clothesy }, { Width, Height }, D2D1::ColorF(0.f, 0.f, 0.f, 0.47f));
	//		OxyRender::Text({ vars::players::clothesx + (Width / 2), vars::players::clothesy + 10 }, D2D1::ColorF::White, true, true, (L"Clothes %s"), TargetPlayerA->_displayName());
	//		OxyRender::Line({ vars::players::clothesx, vars::players::clothesy + 20 }, { vars::players::clothesx + Width, vars::players::clothesy + 20 }, D2D1::ColorF(0.71f, 0.27f, 0.49f), 1);
	//		float Pos = 0;
	//		if (TargetPlayerA->item_list_w()) {
	//			for (int i = 0; i < TargetPlayerA->item_list_w()->get_size(); i++) { // clothes
	//				Item* ActWeapon = TargetPlayerA->item_list_w()->get_value(i);
	//				if (ActWeapon) {
	//					wchar_t* ActiveItem = ActWeapon->get_name();
	//					if (ActiveItem) {
	//						OxyRender::Text({ vars::players::clothesx + (Width / 2), vars::players::clothesy + 40 + Pos }, D2D1::ColorF::White, true, true, (L"%s"), ActiveItem);
	//					}
	//				}
	//				Pos += 15;
	//			}
	//		}
	//	}
	//}

	//if (vars::players::belt && show && !reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)) {
	//	const float Height = 140.f;
	//	const float Width = 160.f;
	//	POINT p;
	//	if (GetCursorPos(&p)) {
	//		if (p.x >= vars::players::beltx && p.x <= vars::players::beltx + Width) {
	//			if (p.y >= vars::players::belty && p.y <= vars::players::belty + Height) {
	//				if (GetAsyncKeyState(VK_LBUTTON) && show) {
	//					vars::players::beltx = p.x - (Width / 2);
	//					vars::players::belty = p.y - (Height / 2);
	//				}
	//			}
	//		}
	//	}
	//	OxyRender::FillRectangle({ vars::players::beltx, vars::players::belty }, { Width, Height }, D2D1::ColorF(0.f, 0.f, 0.f, 0.47f));
	//	OxyRender::Line({ vars::players::beltx, vars::players::belty + 20 }, { vars::players::beltx + Width, vars::players::belty + 20 }, D2D1::ColorF(0.71f, 0.27f, 0.49f), 1);
	//}
	//if (vars::players::clothes && show && !reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)) {
	//	const float Height = 150.f;
	//	const float Width = 160.f;
	//	POINT p;
	//	if (GetCursorPos(&p)) {
	//		if (p.x >= vars::players::clothesx && p.x <= vars::players::clothesx + Width) {
	//			if (p.y >= vars::players::clothesy && p.y <= vars::players::clothesy + Height) {
	//				if (GetAsyncKeyState(VK_LBUTTON) && show) {
	//					vars::players::clothesx = p.x - (Width / 2);
	//					vars::players::clothesy = p.y - (Height / 2);
	//				}
	//			}
	//		}
	//	}
	//	OxyRender::FillRectangle({ vars::players::clothesx, vars::players::clothesy }, { Width, Height }, D2D1::ColorF(0.f, 0.f, 0.f, 0.47f));
	//	OxyRender::Line({ vars::players::clothesx, vars::players::clothesy + 20 }, { vars::players::clothesx + Width, vars::players::clothesy + 20 }, D2D1::ColorF(0.71f, 0.27f, 0.49f), 1);
	//}
	LogSystem::Render();
	if (vars::visuals::raid_esp) {
		LogSystem::RenderExplosions();
	}
	ent_loop();
}