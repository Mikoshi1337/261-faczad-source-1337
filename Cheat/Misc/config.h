#include <string>
#include <vector>
#include <Windows.h>
template <typename T>
class VarType
{
public:
	VarType(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
public:
	CConfig()
	{
		ints = std::vector<VarType<int>*>();
		bools = std::vector<VarType<bool>*>();
		floats = std::vector<VarType<float>*>();
	}

	void Initialize()
	{
		szPath = "C:\\pasta_naxyi\\";
		Setup();
	}

	void Save()
	{
		std::string file;
		file = szPath + vars::stuff::ConfigName + ".pasta_blat";

		CreateDirectoryA(szPath.c_str(), NULL);

		for (VarType<int>* pValue : ints)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<float>* pValue : floats)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<bool>* pValue : bools)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), *pValue->value ? "1" : "0", file.c_str());
	}

	void Remove()
	{
		std::string file;
		file = szPath + vars::stuff::ConfigName + ".cfg";
		std::remove(file.c_str());
	}

	void Load()
	{
		std::string file;
		file = szPath + vars::stuff::ConfigName + ".cfg";
		CreateDirectoryA(szPath.c_str(), NULL);

		char value_l[32] = { '\0' };

		for (VarType<int>* pValue : ints)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = atoi(value_l);
		}

		for (VarType<float>* pValue : floats)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = (float)atof(value_l);
		}

		for (VarType<bool>* pValue : bools)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = !strcmp(value_l, "1");
		}
	}

private:
	void SetupValue(int& value, int def, std::string category, std::string name)
	{
		value = def;
		ints.push_back(new VarType<int>(category, name, &value));
	}

	void SetupValue(bool& value, bool def, std::string category, std::string name)
	{
		value = def;
		bools.push_back(new VarType<bool>(category, name, &value));
	}

	void SetupValue(float& value, float def, std::string category, std::string name)
	{
		value = def;
		floats.push_back(new VarType<float>(category, name, &value));
	}


	void Setup()
	{
		SetupValue(vars::combat::onal, false, ("Combat"), ("HitboxOverride"));
		SetupValue(vars::misc::anti_aim, false, ("Combat"), ("Anti Aim"));
		SetupValue(vars::misc::anti_aim_yaw, false, ("Combat"), ("Anti Aim Type"));
		SetupValue(vars::misc::custom_hitsound, false, ("Combat"), ("Hit Sound"));
		SetupValue(vars::misc::hitsound_rejim, false, ("Combat"), ("Hit Sound Type"));
		SetupValue(vars::visuals::ponan_type, false, ("Combat"), ("Sphere Type"));
		SetupValue(vars::misc::model_mem, false, ("Combat"), ("Model Prank"));
		SetupValue(vars::keys::paste, false, ("Combat"), ("Fly Move"));
		SetupValue(vars::players::namePos, false, ("Visuals"), ("Name Pos"));
		SetupValue(vars::combat::stw, false, ("Combat"), ("stw"));
		SetupValue(vars::combat::bullet_tp, false, ("Combat"), ("bullet_tp"));
		SetupValue(vars::combat::hitscan, false, ("Combat"), ("hitscan"));
		SetupValue(vars::misc::Mautostop, false, ("Combat"), ("Mautostop"));
		SetupValue(vars::combat::manipulate_angle_arrow, false, ("Combat"), ("manipulate_vis"));
		SetupValue(vars::visuals::hand_chams, false, ("Combat"), ("hand_chams"));
		SetupValue(vars::visuals::hand_chams_type, false, ("Combat"), ("hand_chams_type"));
		SetupValue(vars::combat::hammerspam, false, ("Combat"), ("hammerspam"));
		SetupValue(vars::combat::ishak, false, ("Combat"), ("hammerspam"));
		SetupValue(vars::combat::always_heli_rotor, false, ("Combat"), ("Helicopter Magic"));
		SetupValue(vars::combat::silent_melee, false, ("Combat"), ("SilentMelee"));
		SetupValue(vars::combat::autoshoot, false, ("Combat"), ("AutoFire"));
		SetupValue(vars::combat::autoshoot_type, 0, ("Combat"), ("ModelAutoFire"));
		SetupValue(vars::combat::desync_autoshoot_type, 0, ("Combat"), ("AutoFireType"));
		SetupValue(vars::combat::instakill_type, 0, ("Combat"), ("ShotType"));
	    SetupValue(vars::combat::manipulator, false, ("Combat"), ("Manipulator"));
		SetupValue(vars::combat::manipulate_angle, false, ("Combat"), ("Manipulate Angle"));
		SetupValue(vars::combat::autoreload , false, ("Combat"), ("SilentReload"));
		SetupValue(vars::combat::instakill, false, ("Combat"), ("TimeShot"));
		SetupValue(vars::combat::psilent, false, ("Combat"), ("pSilent"));
		SetupValue(vars::combat::visualize_fov, false, ("Combat"), ("DrawFov"));
		SetupValue(vars::combat::fov, 100.f, ("Combat"), ("Fov"));
		SetupValue(vars::combat::ignore_sleepers, false, ("Combat"), ("IgnoreSleepers"));
		SetupValue(vars::combat::ignore_team, false, ("Combat"), ("IgnoreTeam"));
		SetupValue(vars::combat::ignore_npc, false, ("Combat"), ("IgnoreNpc"));
		SetupValue(vars::weapons::recoil_control, 100.f, ("Weapon"), ("RecoilControl"));
		SetupValue(vars::weapons::no_spread, false, ("Weapon"), ("NoSpread"));
		SetupValue(vars::weapons::no_sway, false, ("Weapon"), ("NoSway"));
		SetupValue(vars::weapons::rapid_fire, false, ("Weapon"), ("RapidFire"));
		SetupValue(vars::weapons::automatic, false, ("Weapon"), ("Automatic"));
		SetupValue(vars::weapons::fast_bullets, false, ("Weapon"), ("CBSM"));
		SetupValue(vars::weapons::minicopter_aim, false, ("Weapon"), ("Minicopter Aim"));
		SetupValue(vars::weapons::eokatap, false, ("Weapon"), ("EokaTap"));
		SetupValue(vars::weapons::fastbow, false, ("Weapon"), ("FastBow"));
		SetupValue(vars::weapons::penetrate, false, ("Weapon"), ("Penetrate"));
		SetupValue(vars::players::name, false, ("Players"), ("name"));
		SetupValue(vars::players::box, false, ("Players"), ("box"));
		SetupValue(vars::players::boxstyle, 0, ("Players"), ("boxstyle"));
		SetupValue(vars::players::distance, 0, ("Players"), ("distance"));
		SetupValue(vars::players::skeleton, false, ("Players"), ("skeleton"));
		SetupValue(vars::players::healthbar, false, ("Players"), ("healthbar"));
		SetupValue(vars::players::weapon, false, ("Players"), ("weapon"));
		SetupValue(vars::players::tracers, false, ("Players"), ("tracers"));
		SetupValue(vars::players::tracers_pos, 0, ("Players"), ("TracerPosition"));
		SetupValue(vars::players::targetline_pos, 0, ("Players"), ("TargetTracerPosition"));
		SetupValue(vars::players::targetline, false, ("Players"), ("targetline"));
		SetupValue(vars::players::sleeperignore, false, ("Players"), ("sleeperignore"));

		SetupValue(vars::npc::name, false, ("NPCs"), ("name"));
		SetupValue(vars::npc::box, false, ("NPCs"), ("box"));
		SetupValue(vars::npc::boxstyle, 0, ("NPCs"), ("boxstyle"));
		SetupValue(vars::npc::oof_arrows, false, ("NPCs"), ("oof_arrows"));
		SetupValue(vars::npc::skeleton, false, ("NPCs"), ("skeleton"));
		SetupValue(vars::npc::healthbar, false, ("NPCs"), ("healthbar"));
		SetupValue(vars::npc::weapon, false, ("NPCs"), ("weapon"));
		SetupValue(vars::visuals::zoom, false, ("Visuals"), ("Zoom"));
		SetupValue(vars::misc::fov, 90.f, ("Misc"), ("SexyFuckingFovValue"));
		SetupValue(vars::visuals::target_name, false, ("Visuals"), ("ShowTargetName"));
		SetupValue(vars::misc::bullet_tracers, false, ("Misc"), ("BulletTracers"));
		SetupValue(vars::misc::hitmarker, false, ("Misc"), ("Hitmarker"));
		SetupValue(vars::misc::flyhack_indicator, false, ("Misc"), ("FlyhackIndicator"));
		SetupValue(vars::misc::hit_logs, false, ("Misc"), ("HitLogs"));
		SetupValue(vars::misc::reload_coldown, false, ("Misc"), ("ReloadColdown"));
		SetupValue(vars::combat::instakill_indicator, false, ("Misc"), ("DesyncIndicator"));
		SetupValue(vars::misc::faster_healing, false, ("Misc"), ("FastHeal"));
		SetupValue(vars::misc::fakeadmin, false, ("Misc"), ("FakeAdmin"));
		SetupValue(vars::misc::fast_loot, false, ("Misc"), ("FastLoot"));
		SetupValue(vars::misc::suicide, false, ("Misc"), ("Suicide"));
		SetupValue(vars::misc::no_fall, false, ("Misc"), ("NoFall"));
		SetupValue(vars::misc::can_attack, false, ("Misc"), ("ShootAnywhere"));
		SetupValue(vars::misc::omnidirectional_sprinting, false, ("Misc"), ("OmniSprint"));
		SetupValue(vars::misc::walker, false, ("Misc"), ("NoCollision"));
		SetupValue(vars::misc::weapon_spam, false, ("Misc"), ("WeaponSpam"));
		SetupValue(vars::misc::spiderman, false, ("Misc"), ("Spiderman"));
		SetupValue(vars::misc::inf_jump, false, ("Misc"), ("InfiniteJump"));
		SetupValue(vars::misc::better_jump, false, ("Misc"), ("BetterJump"));
		SetupValue(vars::misc::grade_, 1, ("Misc"), ("Grade"));
		SetupValue(vars::misc::bright_ambient, false, ("Misc"), ("CustomAmbient"));
		SetupValue(vars::misc::custom_time, false, ("Misc"), ("CustomTime"));
		SetupValue(vars::misc::time, 10.f, ("Misc"), ("Time"));
		SetupValue(vars::misc::fly_auto_stopper, false, ("Misc"), ("FlyAutoStopper"));
		SetupValue(vars::keys::suicide, false, ("Keys"), ("suicide"));
		SetupValue(vars::keys::weaponspam, false, ("Keys"), ("weaponspam"));
	    SetupValue(vars::keys::manipulated_key, false, ("Keys"), ("Manipulated"));
		SetupValue(vars::keys::instakill, false, ("Keys"), ("timeshotkey"));
		SetupValue(vars::keys::zoom, false, ("Keys"), ("Zoom"));

		SetupValue(vars::crosshair::cadicall, false, ("Crosshair"), ("Cadicall"));
		SetupValue(vars::crosshair::dot, false, ("Crosshair"), ("Dot"));

		SetupValue(vars::visible::skeleton_color[0], 255, ("colors"), ("skeleton_v r"));
		SetupValue(vars::visible::skeleton_color[1], 0, ("colors"), ("skeleton_v g"));
		SetupValue(vars::visible::skeleton_color[2], 255, ("colors"), ("skeleton_v b"));

		SetupValue(vars::visible::box_color[0], 255, ("colors"), ("box_v r"));
		SetupValue(vars::visible::box_color[1], 0, ("colors"), ("box_v g"));
		SetupValue(vars::visible::box_color[2], 255, ("colors"), ("box_v b"));
	}

	std::string szPath = "";

protected:
	std::vector<VarType<int>*> ints;
	std::vector<VarType<bool>*> bools;
	std::vector<VarType<float>*> floats;
};

CConfig config;