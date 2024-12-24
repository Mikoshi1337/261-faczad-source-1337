int jitter = 1;
int jitter_speed = 10;
int spin_speed = 70;
int spin = 0;

float VFlyhack = 0.0f;
float HFlyhack = 0.0f;
float VMaxFlyhack = 0.0f;
float HMaxFlyhack = 0.0f;
float bigBulletSize = 2.4f;
float flyhackDistanceVertical = 0.f;
float flyhackDistanceHorizontal = 0.f;
float flyhackPauseTime;

bool isreloading = false;
bool just_shot = false;
float fixed_time_last_shot = 0.0f;
bool did_reload = false;
float time_since_last_shot = 0.0f;

bool backgroundm = true;
bool dotdraw = true;
bool init, show = false;
#define null NULL

float indicator_x = 100;
float indicator_y = 100;

std::string steamid = "69";
char spoofid[0x100] = "76561100000000000";

std::string C4 = ("C4");
std::string Satchel = ("Satchel");
std::string IncenRocket = ("Incendiary Rocket");
std::string Rocket = ("Rocket");

std::wstring wC4 = (L"C4");
std::wstring wSatchel = (L"Satchel");
std::wstring wIncenRocket = (L"Incendiary Rocket");
std::wstring wRocket = (L"Rocket");

bool should_interactive = false;
Vector2 screen_center = { 0, 0 };

namespace variables {
	namespace manipulator {
		bool manipulated = false;
		Vector3 m_manipulate = Vector3::Zero();
		float m_last_manipulate_time = 0.0f;
		float desync = 0.0f;
	}
}

namespace vars {
	namespace stuff {
		bool init_infinity = false;
		int font;
		bool gongetflyhack = false;
		float flyhack = 0.f;
		float hor_flyhack = 0.f;
		float max_flyhack = 0.f;
		float max_hor_flyhack = 0.f;
		Vector3 best_target = Vector3(0, 0, 0);
		int anti_aim_ = 0;
		std::wstring dlldirectory;
		bool debugtab = false;
		int testInt = 0;
		float testFloat = 0.f;
		float testFloat3 = 0.f;
		char testChar[0x100] = "testyvesty";
		bool Panic = false;
		int ScreenHeight = 0;
		int ScreenWidth = 0;
		inline float _ScreenHigh = 0;
		inline float _ScreenWidth = 0;
		static std::string selected_cfg = "";
		static int config_index;
		char ConfigName[0x100] = "cfg";
	}
	namespace keys {
		int ModelState{ };
		int desync{ };
		int weaponspam{ };
		int paste{ };
		int zoom = { };
		int teleport = { };
		int suicide = { };
		int longneck = { };
		int lv_key = { };
		int psilent{ };
		int locktarget{ };
		int manipulated_key{ };
		int aimkey{ };
		int debugging{ };
		int ignore_stopper{ };
		int autostopperm{ };
		int bhop{ };
		int speedkey{ };
		int instakill{ };

		int W{ 0x57 };
		int S{ 0x53 };
		int D{ 0x44 };
		int A{ 0x41 };
		int space{ 0x20 };
		inline bool PressedKeys[256] = {};
	}
	namespace stor {
		uintptr_t uBase = NULL;
		uintptr_t gBase = NULL;
		uintptr_t closestPlayer = NULL;
		uintptr_t closestHeli = NULL;
		uintptr_t meme_target = NULL;
	}
	namespace x {
		int angleshow = 0;

	}
	namespace combat {
		bool showang = false;
		bool bodyaimm = false;
		bool davitnudes = false;
		int psilentbone = 0;
		bool bullet_tp = false;
		bool teleport = false;
		bool lv = false;
		bool pierce = false;
		float lv_pidor = 0.300f;
		int ishak = 0;
		int ishak1212 = 0;
		int davitsoso = 0;
		bool paste = false;
		int ishachokmanip = 15;
		bool instanthit = false;
		int fast_b = 1.499f;
		bool stw = false;
		bool visualisempoints = false;
		bool pasta = false;
		bool draw_manip_radius = false;
		bool desync = false;
		bool funkadrochun = false;
		bool autoreload = false;
		int onal = 0;
		bool instakill = false;
		bool instakill_indicator = false;
		bool autoshoot = false;
		int autoshoot_type = 0;
		bool hitscan = false;
		int desync_autoshoot_type = 0;
		int instakill_type = 0;
		bool tree_reflect = false;
		bool always_heli_rotor = false;
		bool always_headshot = false;
		bool silent_melee = false;
		bool hammerspam = false;
		bool hammerspam1 = false;
		bool psilent = false;
		int angle_type = 0;
		bool psilentheli = false;
		bool psilentonkey = false;
		bool aimbot = false;
		bool smooth = false;
		int smooth_factor = 1;
		float reach = 3.f;
		float range = 400.f;
		int fov = 100.f;
		bool bodyaim = false;
		bool visualize_fov = false;
		bool visualize_fovv = false;
		bool visualize_prediction = false;
		bool lock_target = false;
		bool manipulator = false;
		bool bulletspec = false;
		bool manipulate_angle = false;
		bool manipulate_angle_arrow = false;
		bool sphere = false;
		bool ignore_players = false;
		bool ignore_heli = false;
		bool ignore_invisible = false;
		bool ignore_sleepers = false;
		bool ignore_team = true;
		bool ignore_npc = false;

	}
	namespace weapons {
		bool no_recoil = false;
		int recoil_control = 100.f;
		bool fast_bullets = false;
		bool penetrate = false;
		bool minicopter_aim = false;
		bool spoof_hitdistance = false;
		float hitdistance = 100.f;
		bool remove_attack_anim = false;
		float bulletspeed = 1.00f;
		bool no_spread = false;
		bool automatic = false;
		bool thick_bullet = false;
		float thickness = 1.0f;
		Vector3 BulletPos = Vector3(0, 0, 0);
		bool ready = false;
		bool no_sway = false;
		bool rapid_fire = false;
		bool fastbow = false;
		bool eokatap = false;
		bool compound = false;
		bool local_scale = false;
		float bullet_sizex = 1.f;
		float bullet_sizey = 1.f;
		float bullet_sizez = 1.f;
	}
	namespace players {
		int fovstyle = 0;
		bool wounded = false;
		int weaponPos = 0;
		int momentedline_type = 0;
		int Outstyle = 0;
		int fontstyle = 0;
		int bullettrace_type = 0;
		int chams_type = 0;
		bool target_tracers = false;
		bool oof_arrows = false;
		int namePos = 0;
		bool sphere = false;
		float uptime = 2.f;
		bool distance = false;
		bool chams_ = false;
		bool chams = false;
		float VisRcolor = 1;
		float VisGcolor = 0.6;
		float VisBcolor = 0;
		bool chams_player = false;
		bool chams_sleeper = false;
		bool chams_npc = false;
		float xc = 3.f;
		float yc = 3.f;
		float zc = 0.f;
		bool OnlyVisible = false;
		bool health_chams = false;
		bool rainbow_chams = false;
		bool belt = false;
		bool clothes = false;
		float beltx = 50.f;
		float belty = 50.f;
		float clothesx = 200.f;
		float clothesy = 50.f;
		bool box = false;
		int boxstyle = 0;
		bool skeleton = false;
		bool name = false;
		bool looking_direction = false;
		bool healthdist = false;
		bool healthbar = false;
		int healthbarstyle = 0;
		int healthbarscolortyle = 0;
		int healthbarscolor = 0;
		bool weapon = false;
		bool sleeperignore = false;
		bool tracers = false;
		int tracers_pos = 0;
		bool targetline = false;
		int targetline_pos = 0;
		bool fillbox = false;
	}
	namespace npc {
		bool distance = false;
		bool chams_xqz = false;
		bool chams = false;
		bool oof_arrows = false;
		bool box = false;
		int boxstyle = false;
		bool skeleton = false;
		bool name = false;
		bool looking_direction = false;
		bool healthdist = false;
		bool healthbar = false;
		int healthbarstyle = 0;
		int healthbarscolortyle = 0;
		int healthbarscolor = 0;
		bool weapon = false;
		bool tracers = false;
		bool fillbox = false;
	}
	namespace ores {
		bool closest_ore = false;
		bool show_collectables = false;
		bool stone = false;
		bool sulfur = false;
		bool metal = false;
		bool show_distance = false;
		float draw_distance = 300.f;
	}
	namespace visuals {
		int hand_chams_type = 0;
		int chams_render_mode = 0;
		bool wrn = false;
		bool ponan = false;
		bool hand_chams = false;
		bool keybinds = false;
		bool waterka = true;
		bool GameSense = false;
		int ponan_type = false;

		bool zoom = false;
		bool target_name = false;
		bool Russia = false;
		bool Latvia = false;
		bool raid_esp = false;
		bool bradley_apc = false;
		bool patrol_heli = false;
		bool radar_ = false;
		namespace radar {
			float x = 300.f;
			float y = 300.f;
			float range = 160.f;
			float size = 160.f;
		}
		namespace base {
			bool tc = false;
			bool sleeping_bag = false;
			bool bed = false;
			bool boxes = false;
			bool show_distance = false;
			float draw_distance = 300.f;
		}
		namespace vehicles {
			bool minicopter = false;
			bool scrapheli = false;
			bool boat = false;
			bool rhib = false;
			bool show_distance = false;
			float draw_distance = 300.f;
		}
		namespace turrets {
			bool auto_turret = false;
			bool flame_turret = false;
			bool shotgun_turret = false;
			bool landmine = false;
			bool sam_site = false;
			bool bear_trap = false;
			bool show_distance = false;
			float draw_distance = 300.f;
		}
		namespace other {
			bool dropped_items = false;
			bool bodybag = false;
			bool corpse = false;
			bool stash = false;
			bool hemp = false;
			bool show_distance = false;
			float draw_distance = 300.f;
		}
		namespace crates {
			bool elite = false;
			bool military = false;
			bool supply = false;
			bool chinook = false;
			bool heli = false;
			bool bradley = false;
			bool show_distance = false;
			float draw_distance = 300.f;
		}
		namespace animals {
			bool bear = false;
			bool pig = false;
			bool chicken = false;
			bool wolf = false;
			bool deer = false;
			bool show_distance = false;
			float draw_distance = 300.f;
		}
	}
	namespace misc {
		float max;
		bool eyeline = false;
		bool AspectRatio = false;
		int ratio = 1.4f;
		int hitmaterial = 0;
		int hitsound_rejim = 0;
		int modelstate = 0;
		float fotsize = 12.f;
		bool wt = true;
		bool model_mem = false;
		bool dodge_proj = false;
		bool recycler = false;
		bool farm_assist = false;
		bool bhop = false;
		bool fly_auto_stopper = false;
		bool Mautostop = false;
		bool interactive_debug = false;
		bool weapon_spam = false;
		bool bullet_tracers = false;
		bool movement_line = false;
		bool local_bones = false;
		bool fast_loot = false;
		bool flyhack_indicator = false;
		bool reload_coldown = false;
		bool farmbot = false;
		bool jumpfarm = false;
		bool farmbot_barrels = false;
		bool farmbot_ore = false;
		bool farmbot_trees = false;
		bool spiderman = false;
		bool better_jump = false;
		bool no_fall = false;
		bool anti_aim_indicator = false;
		bool hit_logs = false;
		bool auto_farm_ore = false;
		bool auto_farm_tree = false;
		bool can_attack = false;
		bool walker = false;
		bool auto_grade = false;
		int grade_ = 0;
		bool faster_healing = false;
		bool suicide = false;
		bool mass_suicide = false;
		bool long_neck = false;
		bool inf_jump = false;
		bool auto_revive = false;
		bool revive_team_only = false;
		bool rayleigh_changer = false;
		float rayleigh = 10.f;
		bool mie_changer = false;
		float mie = 10.f;
		bool brightness_changer = false;
		float brightness = 10.f;
		bool annoyer = false;
		bool bright_ambient = false;
		bool night_stars = false;
		float stars = 1000.f;
		bool auto_farm = false;
		bool auto_farm_barrel = false;
		bool egg_bot = false;
		bool auto_pickup = false;
		bool anti_aim = false;
		int anti_aim_speed = 10;
		int anti_aim_yaw = false;
		int fake_lag_type = false;
		bool fake_lag = false;
		bool silent_walk = false;
		int fov = 90.f;
		bool gravity = false;
		float gravity_modifier = 0.f;
		bool custom_hitsound = false;
		bool omnidirectional_sprinting = false;
		bool fakeadmin = false;
		bool custom_time = false;
		int time = 10.0f;
		bool hitmarker = false;
		bool speedhack = false;
		float speeding = 0.f;
	}
	namespace crosshair {
		bool plusminus = false;
		bool default = false;
		bool swastika = false;
		float speed = 2.0f;
		float razmer = 10;
		bool cadicall = false;

		bool custom = false;
		bool dot = false;
		float gap = 5.f;
		float length = 5.f;
	}
	namespace colors {
		float target_name_color[] = { 255.f, 255.f, 255.f };
		float log_name_color[] = { 255.f, 255.f, 255.f };
		float sleep_color[] = { 1.f, 1.f, 0.f };
		float team_color[] = { 0.f, 255.f, 0.f };
		float oof_color[] = { 255.f, 255.f, 255.f };
		float prediction_color[] = { 255.f, 0.f, 0.f };
		float default_color[] = { 0.f, 0.f, 0.f };
		float custom_color[] = { 255.f, 255.f, 255.f };
		float cadicall_color[] = { 255.f, 255.f, 255.f };
		float dot_color[] = { 255.f, 255.f, 255.f };
		float swaston_color[] = { 255.f, 255.f, 255.f };
		float flyhack_color[] = { 255.f, 0.f, 0.f };
		float fov_color[] = { 120.f, 255.f, 120.f };
		float gradient_1[] = { 255.f, 255.f, 255.f };
		float gradient_2[] = { 255.f, 255.f, 255.f };
		float npc_name_color[] = { 255.f, 255.f, 255.f };
		float npc_healthdist_color[] = { 255.f, 255.f, 255.f };
		float npc_weapons_color[] = { 255.f, 255.f, 255.f };
		float npc_skeleton_color[] = { 255.f, 255.f, 255.f };
		float npc_box_color[] = { 255.f, 255.f, 255.f };
		float npc_tracers_color[] = { 255.f, 255.f, 255.f };
		float npc_custom_healthColor[] = { 255.f, 255.f, 255.f };
		float Player_custom_healthColor[] = { 255.f, 255.f, 255.f };
		ImVec4 backtrack_col = { 0.f, 1.f, 1.f, 1.f };
		ImVec4 mov_line = { 255.f, 0.f, 255.f, 255.f };
		ImVec4 local_bones = { 1.f, 0.f, 0.f, 255.f };
		ImVec4 marker = { 255.f, 255.f, 0.f, 255.f };
		ImVec4 tracers = { 0.f, 1.0f, 1.0f, 1.0f };
		ImVec4 chamsinvusuble = { 0, 3, 3, 1 };
		ImVec4 chamsvusuble = { 0, 3, 3, 1 };
		ImVec4 chams = { 0, 3, 3, 1 };
		ImVec4 hand_chams = { 0, 3, 3, 1 };
		ImVec4 pasta = { 255.f, 0.f, 255.f, 255.f };
		ImVec4 chams_inv = { 0, 3, 3, 1 };
		ImVec4 ambient_color = { 0.8f, 0.8f, 0.8f, 0.8f };
		ImVec4 angle = { 1.f, 0.f, 0.f, 1.0f };
		ImVec4 angle_arrow = { 1.f, 0.5f, 0.f, 1.f };
	}
	namespace visible {
		float skeleton_color[] = { 255.f, 0.f, 255.f };
		float visualise[] = { 0.f, 239.f, 248.f };
		float box_color[] = { 255.f, 0.f, 255.f };
	}
	namespace invisible {
		float skeleton_color[] = { 0.f, 255.f, 255.f };
		float box_color[] = { 0.f, 255.f, 255.f };
	}
}