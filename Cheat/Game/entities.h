#include <random>
#pragma comment(lib, "Urlmon.lib")
using namespace otherEsp;
bool inited = false;
void ent_loop() {
	DrawList = ImGui::GetBackgroundDrawList();
	static float r = 1.00f, g = 0.00f, b = 1.00f;
	static int cases = 0;
	switch (cases)
	{
	case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
	case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
	case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
	case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
	default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
	}
	uintptr_t bn = read(vars::stor::gBase + CO::BaseNetworkable, uintptr_t);
	if (!bn) {
		return;
	}
	else {
		SYSTEMTIME st;
		GetLocalTime(&st);
		float sWidth = viewport.Width;
	}
	if (!inited) {
		CreateDirectoryA(("C:\\CosmoRust"), NULL);

		config.Initialize();

		inited = true;
	}
	if (LocalPlayer::Entity()->HasPlayerFlag(Connected)) return;

	static float faken_rot = 0.0f;
	static int gamerjuice = 0;
	float a = screen_center.y / 30.0f;
	float gamma = atan(a / a);

	if (vars::combat::instakill_indicator) {
		float xs = vars::stuff::ScreenWidth / 2, ys = vars::stuff::ScreenHeight / 2;
		float size = 150;
		if (vars::keys::manipulated_key)
		{
			g_Render->FilledRect(xs - 70, ys - 450, size, 6.f, ImColor(0.f, 0.f, 0.f, 1.f), 0, 0);
		}
		if (vars::combat::instakill_indicator) {
			auto desynctime = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime()) - 0.03125 * 3;
			auto desyncpercentage = (((desynctime / 1.00f) * 120.0f) + 1.f) / 120;
			float red, green, blue;
			float Num = desyncpercentage;
			if (desyncpercentage < 0.05) Num = 0;
			if (vars::combat::instakill_indicator && Num != 0) {
				if (Num < 0.5) {
					red = Num * 2.f * 255.f;
					green = 255.f;
					blue = 0.f;
				}
				else {
					red = 255.f;
					green = (2.f - 2.f * Num) * 255.f;
					blue = 0.f;
				}

				if (Num > 1.f)
					Num = 1.f;

				ImVec2 center(vars::stuff::ScreenWidth / 2, vars::stuff::ScreenHeight / 2);
				float size = 120;
				float x = (desynctime * size);
				if (x > (size) * 2)
					x = (size) * 2; //s

				if (vars::keys::manipulated_key)
				{
					g_Render->FilledRect(xs - 70, ys - 450, x - 2, 6.f, ImColor(accent_color[0], accent_color[1], accent_color[2]), 0, 0);
				}
			}
		}
		if (vars::keys::manipulated_key)
		{
			g_Render->Rect(xs - 70, ys - 450, size, 6.f, ImColor(0.f, 0.f, 0.f, 1.f), 0, 0);
		}
	}

	if (vars::misc::reload_coldown) {
		float xs = vars::stuff::ScreenWidth / 2, ys = vars::stuff::ScreenHeight / 2;
		auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
		float size = 120;
		if (held) {
			if (did_reload == false && time_since_last_shot <= (held->reloadTime() - (held->reloadTime() / 10)) && time_since_last_shot > 0) {
				float reloadDurationTotal = (held->reloadTime() - (held->reloadTime() / 10));
				float reloadDuration = time_since_last_shot;
				auto percentage = (((reloadDuration / reloadDurationTotal) * 100.0f) + 1.f) / 100;
				float red, green, blue;
				float Num = percentage;
				if (vars::misc::reload_coldown && Num != 0) {
					if (Num < 0.5) {
						red = Num * 2.f * 255.f;
						green = 255.f;
						blue = 0.f;
					}
					else {
						red = 255.f;
						green = (2.f - 2.f * Num) * 255.f;
						blue = 0.f;
					}
					if (Num > 1.f) Num = 1.f;
					ImVec2 center(vars::stuff::ScreenWidth / 2, vars::stuff::ScreenHeight / 2);
					float size = 120;
					float x = (percentage * size);
					if (x > (size) * 2)
						x = (size) * 2; //s
					g_Render->DrawString(xs, ys - 80, ImColor(255, 255, 255), render2::outline | render2::centered_x, calibri_, 13, ("reloading"));
				}
			}
			else {
				if (held->HasReloadCooldown()) {
					float SamReloadDuration = held->nextReloadTime() - GLOBAL_TIME;
					float SamReloadDurationTotal = held->CalculateCooldownTime(held->nextReloadTime(), held->reloadTime()) - GLOBAL_TIME;
					auto SamPercentage = (((SamReloadDuration / SamReloadDurationTotal) * 100.0f) + 1.f) / 100;
					float red, green, blue;
					float Num = SamPercentage;
					if (vars::misc::reload_coldown && Num != 0) {
						if (Num < 0.5) {
							red = Num * 2.f * 255.f;
							green = 255.f;
							blue = 0.f;
						}
						else {
							red = 255.f;
							green = (2.f - 2.f * Num) * 255.f;
							blue = 0.f;
						}

						if (Num > 1.f)
							Num = 1.f;

						ImVec2 center(vars::stuff::ScreenWidth / 2, vars::stuff::ScreenHeight / 2);
						float size = 120;
						float x = (SamPercentage * size);
						if (x > (size) * 2)
							x = (size) * 2; //s
					}
				}
			}
			if (held->class_name_hash() == STATIC_CRC32("BaseProjectile") ||
				held->class_name_hash() == STATIC_CRC32("BowWeapon") ||
				held->class_name_hash() == STATIC_CRC32("CompoundBowWeapon") ||
				held->class_name_hash() == STATIC_CRC32("BaseLauncher") ||
				held->class_name_hash() == STATIC_CRC32("CrossbowWeapon")) {

				float SamReloadDuration = held->nextReloadTime() - GLOBAL_TIME;
				float SamReloadDurationTotal = held->CalculateCooldownTime(held->nextReloadTime(), held->reloadTime()) - GLOBAL_TIME;
				auto SamPercentage = (((SamReloadDuration / SamReloadDurationTotal) * 100.0f) + 1.f) / 100;
				float x = (SamPercentage * size);

				if (x > (size) * 2)
					x = (size) * 2; //s
			}
		}
	}

	


	float FOV = vars::combat::fov, CurFOV;
	auto entityList = BaseNetworkable::clientEntities()->entityList();
	if (entityList) {
		for (int i = 0; i < entityList->vals->size; i++) {
			uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			auto entity = *reinterpret_cast<BaseEntity**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			if (!Entity) continue;
			uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
			if (!Object) continue;
			uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
			if (!ObjectClass) continue;

			pUncStr name = read(ObjectClass + 0x60, pUncStr); if (!name) continue;
			char* buff = name->stub;

			if (!reinterpret_cast<BaseEntity*>(Entity)->IsValid()) { continue; }
			if (!reinterpret_cast<Component*>(Entity)->gameObject()) { continue; }
			if (strstr(buff, ("Local"))) { continue; }

			if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer()) {
				BasePlayer* Player = (BasePlayer*)Entity;

				if (Player->lifestate() == BaseCombatEntity::LifeState::Dead) continue;

				if (vars::players::skeleton && !Player->IsNpc()) {
					if (!Player->get_flag(PlayerFlags::Sleeping)) {
						if (LocalPlayer::Entity()->is_teammate(Player->userID())) {
							Skeleton(Player, ImColor(0, 255, 0));
						}
						else {
							Skeleton(Player, ImColor(vars::visible::skeleton_color));
						}
					}
					else if (Player->get_flag(PlayerFlags::Sleeping) && !vars::players::sleeperignore) {
						Skeleton(Player, ImColor(vars::colors::sleep_color));
					}
				}
				else if (vars::npc::skeleton && Player->IsNpc()) {
					Skeleton(Player, ImColor(255, 255, 0));
				}

				ImColor color = ImColor(0.f, 0.f, 0.f, 1.f);
				if (!Player->IsNpc()) {
					if (!Player->get_flag(PlayerFlags::Sleeping)) {
						if (LocalPlayer::Entity()->is_teammate(Player->userID())) {
							color = ImColor(vars::colors::team_color);
						}
						else {
							color = ImColor(vars::visible::box_color);
						}
					}
					else {
						color = ImColor(vars::colors::sleep_color);
					}
					ESP(Player, color);
				}
				else if (Player->IsNpc()) {
					color = ImColor(255, 255, 0);
					if (Player->health() > 0) {
						NPCESP(Player, ImColor(255, 255, 0));
					}
				}
				else {
					color = ImColor(255, 255, 0);
				}
				ImColor _color = ImColor(0.f, 0.f, 0.f, 1.f);
				if (!Player->IsNpc()) {
					if (!Player->get_flag(PlayerFlags::Sleeping)) {
						if (LocalPlayer::Entity()->is_teammate(Player->userID())) {
							_color = ImColor(vars::colors::team_color);
						}
						else {
							if (Player->health() <= 0) {
								_color = ImColor(1.f, 0.f, 0.f, 1.f);
							}
							else {
								_color = ImColor(vars::visible::box_color);
							}
						}
					}
					else {
						_color = ImColor(vars::colors::sleep_color);
					}
				}
				OOF(Player, _color);

				if (vars::combat::ignore_sleepers && Player->get_flag(PlayerFlags::Sleeping)) continue;
				if (vars::combat::ignore_npc && Player->IsNpc()) continue;
				if (vars::combat::ignore_team && LocalPlayer::Entity()->is_teammate(Player->userID())) continue;
				if (Player->get_bone_pos(head).x == 0 || Player->get_bone_pos(head).y == 0 || Player->get_bone_pos(head).z == 0) continue;
				if (vars::combat::ignore_players) continue;
				if (Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), Player->get_bone_pos(head)) > vars::combat::range) continue;

				if (FOV > (CurFOV = GetFov(Player, BoneList(0.5))) && Player->health() > 0 && !vars::combat::lock_target) {
					FOV = CurFOV; vars::stor::closestPlayer = (uintptr_t)Player;
				}
			}
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->ClassNameHash() == STATIC_CRC32("BaseHelicopter")) {
				BaseEntity* helicopter = (BaseEntity*)Entity;
				if (helicopter) {
					float health = reinterpret_cast<BaseCombatEntity*>(Entity)->health();
					float maxhealth = 10000.f;
					Vector3 pos = helicopter->transform()->position();
					Vector2 screenPos;
					if (utils::w2s(pos, screenPos)) {
						if (vars::visuals::patrol_heli) {
							std::string string;
							char buf[256] = { 0 }; sprintf_s(buf, ("Helicopter[%1.0fm]"), Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos));
							string += buf;
							std::string string2;
							char buf2[256] = { 0 }; sprintf_s(buf2, ("[%dHP]"), (int)health);
							string2 += buf2;
							g_Render->DrawString(screenPos.x, screenPos.y, ImColor(255, 255, 255), render2::outline|render2::centered_x, pixel_s, 11, string.c_str());
							g_Render->DrawString(screenPos.x, screenPos.y + 15, ImColor(255, 255, 25), render2::outline|render2::centered_x, pixel_s, 11, string2.c_str());
			
							g_Render->FilledRect(screenPos.x - 30, screenPos.y + 20, 60, 6, ImColor(0, 0, 0), 0, 0);
							g_Render->FilledRect(screenPos.x - 30, screenPos.y + 20, 60 * (health / maxhealth), 6, ImColor(0,255,0),0,0);
							g_Render->Rect(screenPos.x - 30, screenPos.y + 20, 60, 6, ImColor(0, 0, 0), 0, 0);
						}
						if (health > 0 && !vars::combat::ignore_heli) {
							if ((FOV > (CurFOV = GetFovHeli(pos)))) {
								FOV = CurFOV; vars::stor::closestHeli = (uintptr_t)helicopter;
							}
						}
					}
				}
			}
			otherEsp::bradley(ObjectClass, Entity, buff);
			otherEsp::corpse(ObjectClass, Entity, buff);
			otherEsp::tc(ObjectClass, Entity, buff);
			otherEsp::sleepingbag(ObjectClass, Entity, buff);
			otherEsp::bed(ObjectClass, Entity, buff);
			otherEsp::stash(ObjectClass, Entity, buff);
			otherEsp::hackablecrate(ObjectClass, Entity, buff);
			//otherEsp::world(ObjectClass, Entity, buff);
			if (vars::ores::show_collectables) {
				miscvis(Entity, buff, vars::ores::stone, vars::ores::show_distance, vars::ores::draw_distance, ("stone-collectable.prefab"), (L"Stone Collectable"), ImColor(128, 128, 128));
				miscvis(Entity, buff, vars::ores::sulfur, vars::ores::show_distance, vars::ores::draw_distance, ("sulfur-collectable.prefab"), (L"Sulfur Collectable"), ImColor(255, 215, 0));
				miscvis(Entity, buff, vars::ores::metal, vars::ores::show_distance, vars::ores::draw_distance, ("metal-collectable.prefab"), (L"Metal Collectable"), ImColor(139, 69, 19));
			}
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::ores::stone, vars::ores::show_distance, vars::ores::draw_distance, ("stone-ore.prefab"), (L"Stone Ore"), ImColor(128, 128, 128));
			miscvis(Entity, buff, vars::ores::sulfur, vars::ores::show_distance, vars::ores::draw_distance, ("sulfur-ore.prefab"), (L"Sulfur Ore"), ImColor(255, 215, 0));
			miscvis(Entity, buff, vars::ores::metal, vars::ores::show_distance, vars::ores::draw_distance, ("metal-ore.prefab"), (L"Metal Ore"), ImColor(139, 69, 19));
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::crates::elite, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, ("crate_elite.prefab"), (L"Elite Crate"), ImColor(46, 139, 87));
			miscvis(Entity, buff, vars::visuals::crates::military, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, ("crate_normal.prefab"), (L"Military Crate"), ImColor(1, 68, 33));
			miscvis(Entity, buff, vars::visuals::crates::supply, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, ("supply_drop.prefab"), (L"Airdrop"), ImColor(0, 139, 139));
			miscvis(Entity, buff, vars::visuals::crates::heli, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, ("heli_crate.prefab"), (L"Heli Crate"), ImColor(1, 50, 32));
			miscvis(Entity, buff, vars::visuals::crates::bradley, vars::visuals::crates::show_distance, vars::visuals::crates::draw_distance, ("bradley_crate.prefab"), (L"Bradley Crate"), ImColor(173, 255, 47));
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::vehicles::minicopter, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, ("minicopter.entity.prefab"), (L"Minicopter"), ImColor(0, 0, 255));
			miscvis(Entity, buff, vars::visuals::vehicles::scrapheli, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, ("scraptransporthelicopter.prefab"), (L"Scrap Heli"), ImColor(0, 0, 139));
			miscvis(Entity, buff, vars::visuals::vehicles::boat, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, ("rowboat.prefab"), (L"Boat"), ImColor(173, 216, 230));
			miscvis(Entity, buff, vars::visuals::vehicles::rhib, vars::visuals::vehicles::show_distance, vars::visuals::vehicles::draw_distance, ("rhib.prefab"), (L"RHIB"), ImColor(224, 255, 255));
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::turrets::auto_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, ("autoturret_deployed.prefab"), (L"Auto Turret"), ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::turrets::flame_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, ("flameturret.deployed.prefab"), (L"Flame Turret"), ImColor(255, 140, 0));
			miscvis(Entity, buff, vars::visuals::turrets::shotgun_turret, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, ("guntrap.deployed.prefab"), (L"Shotgun Trap"), ImColor(105, 105, 105));
			miscvis(Entity, buff, vars::visuals::turrets::landmine, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, ("landmine.prefab"), (L"Landmine"), ImColor(138, 43, 226));
			miscvis(Entity, buff, vars::visuals::turrets::sam_site, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, ("sam_site_turret_deployed.prefab"), (L"SAM Site"), ImColor(176, 224, 230));
			miscvis(Entity, buff, vars::visuals::turrets::bear_trap, vars::visuals::turrets::show_distance, vars::visuals::turrets::draw_distance, ("beartrap.prefab"), (L"Beartrap"), ImColor(255, 248, 220));
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::other::hemp, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, ("hemp-collectable.prefab"), (L"Hemp"), ImColor(50, 205, 50));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "rifle.ak", L"AK-47 Rifle", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "rifle.lr300", L"LR300 Rifle", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "rifle.l96", L"L96 Sniper", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "rifle.m39", L"M39 Rifle", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "pistol.prototype17", L"Prototype 17", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "bow.compound	", L"Bow Compound", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "rifle.semiauto", L"Semi Auto Rifle", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "smg.mp5", L"MP5 SMG", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "smg.2", L"SMG Auto", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "smg.thompson", L"Smg Thompson", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "pistol.nailgun", L"Nailgun Pistol", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "pistol.eoka", L"EOKA Pistol", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "pistol.m92", L"M92 Pistol", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "pistol.python", L"Python Pistol", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "pistol.revolver", L"Revolver Pistol", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "pistol.semiauto", L"Semi Auto Pistol", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "shotgun.pump", L"Shotgun Pump", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "shotgun.double", L"Double Shotgun", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "grenade.f1", L"F1 Grenade", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "multiplegrenadelauncher", L"Multiple Grenade Launcher", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "bow.compound", L"Compound Bow", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "crossbow", L"Crossbow", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "bow.hunting", L"Bow", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "hmlmg", L"HMLMG", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "spear.stone", L"Spear Stone", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "spear.wooden", L"Spear Wooden", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "shotgun.double", L"Double Shotgun", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "ammo.nailgun.nails", L"Nailgun Nails", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "arrow.hv", L"Arrow Hv", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "arrow.bone", L"Arrow Bone", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "arrow.fire", L"Arrow Fire", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::dropped_items, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, "arrow.wooden", L"Arrow wooden", ImColor(255, 165, 0));
			miscvis(Entity, buff, vars::visuals::other::bodybag, vars::visuals::other::show_distance, vars::visuals::other::draw_distance, ("item_drop_backpack.prefab"), (L"Bodybag"), ImColor(139, 0, 139));
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::base::boxes, vars::visuals::base::show_distance, vars::visuals::base::draw_distance, ("box.wooden.large.prefab"), (L"Box"), ImColor(188, 143, 143));
			// ---------------------------------------------------------
			miscvis(Entity, buff, vars::visuals::animals::bear, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, ("bear.prefab"), (L"Bear"), ImColor(139, 69, 19));
			miscvis(Entity, buff, vars::visuals::animals::wolf, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, ("wolf.prefab"), (L"Wolf"), ImColor(119, 136, 153));
			miscvis(Entity, buff, vars::visuals::animals::pig, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, ("boar.prefab"), (L"Pig"), ImColor(255, 136, 77));
			miscvis(Entity, buff, vars::visuals::animals::chicken, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, ("chicken.prefab"), (L"Chicken"), ImColor(173, 255, 47));
			miscvis(Entity, buff, vars::visuals::animals::deer, vars::visuals::animals::show_distance, vars::visuals::animals::draw_distance, ("horse.prefab"), (L"Horse"), ImColor(244, 164, 96));
		}
		BasePlayer* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
		auto transform = reinterpret_cast<BasePlayer*>(TargetPlayer)->get_bone_pos(head);
		Quaternion q;
		if (vars::visuals::ponan_type == 0)
		{
			
		}
		if (vars::visuals::ponan_type == 1)
		{
			DDraw::Sphere(transform, 2.6f, Color(1.f, 1.f, 1.f, 255.f), 0.01f, false);
		}
		if (vars::visuals::ponan_type == 2)
		{
			DDraw::SphereGizmoZV(transform, 2.6f, Color(1.f, 1.f, 1.f, 255.f), 0.01f, false, true);
		}
		if (vars::visuals::ponan_type == 3)
		{
			DDraw::Box(transform,2.6f, Color(1.f, 1.f, 1.f, 255.f), 0.01f, false);
		}

		BasePlayer* TargetPlayer1 = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
		if (TargetPlayer) {
			if (TargetPlayer->get_bone_pos(head).x == 0 || TargetPlayer->get_bone_pos(head).y == 0 || TargetPlayer->get_bone_pos(head).z == 0) {
				vars::stor::closestPlayer = NULL;
			}
			if (TargetPlayer->IsNpc() && vars::combat::ignore_npc) {
				vars::stor::closestPlayer = NULL;
			}
			if (TargetPlayer->get_flag(PlayerFlags::Sleeping) && vars::combat::ignore_sleepers) {
				vars::stor::closestPlayer = NULL;
			}
			if (LocalPlayer::Entity()->is_teammate(TargetPlayer->userID()) && vars::combat::ignore_team) {
				vars::stor::closestPlayer = NULL;
			}
			if (TargetPlayer->health() <= 0) {
				vars::combat::lock_target = false;
				vars::stor::closestPlayer = NULL;
			}
		}
		if (vars::stor::closestPlayer == NULL) {
			vars::combat::lock_target = false;
		}
		if (vars::stor::closestHeli != NULL) {
			if (reinterpret_cast<BaseEntity*>(vars::stor::closestHeli)->IsDestroyed()) {
				vars::stor::closestHeli = NULL;
			}
			if (!reinterpret_cast<Component*>(vars::stor::closestHeli)->gameObject()) {
				vars::stor::closestHeli = NULL;
			}
		}
		if (vars::combat::ignore_players) {
			vars::stor::closestPlayer = NULL;
		}
		if (vars::combat::ignore_heli) {
			vars::stor::closestHeli = NULL;
		}
		if (LocalPlayer::Entity() != nullptr) {
			if (vars::combat::aimbot && !LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->userID())) {
				if (vars::stor::closestPlayer) {
					if (GetAsyncKeyState(vars::keys::aimkey)) {
						do_aimbot(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer));
					}
				}
			}
		}
	}
}