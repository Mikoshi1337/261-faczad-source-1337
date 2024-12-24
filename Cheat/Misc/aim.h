#include <map>
namespace a {
	double CalcBulletDrop(double height, double DepthPlayerTarget, float velocity, float gravity) {

		auto* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
		auto transform = TargetPlayer->get_bone_pos(head);

		auto distance = (int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), TargetPlayer->get_bone_pos(head));

		double pitch = (atan2(height, DepthPlayerTarget));
		double BulletVelocityXY = velocity * cos(pitch);
		double Time = DepthPlayerTarget / BulletVelocityXY;
		if (GetAsyncKeyState(vars::keys::lv_key)) {
			if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1545779598);
			
				if (distance < 55.f) {
					double TotalVerticalDrop = (0.5465 * gravity * Time * Time);
					//return TotalVerticalDrop * 10;
				}
				else if (distance > 56.f) {
					double TotalVerticalDrop = (0.5465 * gravity * Time * Time);
					//return TotalVerticalDrop * 10;
				}
				else if (distance > 78.f) {
					double TotalVerticalDrop = (0.56 * gravity * Time * Time);
					//return TotalVerticalDrop * 10;
				}

			}
		if (GetAsyncKeyState(vars::keys::lv_key)) {
			if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -1335497659);

			if (distance < 55.f) {
				double TotalVerticalDrop = (0.5465 * gravity * Time * Time);
				//return TotalVerticalDrop * 10;
			}
			else if (distance > 56.f) {
				double TotalVerticalDrop = (0.5465 * gravity * Time * Time);
				//return TotalVerticalDrop * 10;
			}
			else if (distance > 78.f) {
				double TotalVerticalDrop = (0.56 * gravity * Time * Time);
				//return TotalVerticalDrop * 10;
			}

		}

		
		
		if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1545779598) 
		{
			double TotalVerticalDrop = 1.000000 * 0.600000;

			if (distance > 400.f) {
				double TotalVerticalDrop = (0.751 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 350.f) {
				double TotalVerticalDrop = (0.745 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 340.f) {
				double TotalVerticalDrop = (0.7355 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 330.f) {
				double TotalVerticalDrop = (0.731 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 300.f) {
				double TotalVerticalDrop = (0.716 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 250.f) {
				double TotalVerticalDrop = (0.622 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 200.f) {
				double TotalVerticalDrop = (0.5941 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 150.f) {
				double TotalVerticalDrop = (0.5727 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance > 100.f) {
				double TotalVerticalDrop = (0.567 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}
			if (distance < 100.f) {
				double TotalVerticalDrop = (0.569 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}

		}

		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1318558775) 
		{
			double TotalVerticalDrop = 1.000000 * 0.700000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -1812555177) 
		{
			double TotalVerticalDrop = 1.000000 * 0.600000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -904863145) 
		{
			double TotalVerticalDrop = 1.000000 * 0.600000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1796682209) 
		{
			double TotalVerticalDrop = 1.000000 * 0.700000;
		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1373971859) 
		{
			double TotalVerticalDrop = 1.000000 * 0.700000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 818877484)
		{
			double TotalVerticalDrop = 1.000000 * 0.700000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -1123473824)
		{
			if (distance < 100.f) {
				double TotalVerticalDrop = (0.58 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 28201841) 
		{
			double TotalVerticalDrop = 1.000000 * 0.600000;
		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 884424049) 
		{
			double TotalVerticalDrop = 0.750000 * 0.004500;
		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 649912614) 
		{
			double TotalVerticalDrop = 1.000000 * 0.700000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -765183617) 
		{
			double TotalVerticalDrop = 1.000000 * 1.000000;
		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -75944661) 
		{
			double TotalVerticalDrop = -2;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1953903201) 
		{
			if (distance < 55.f) {
				double TotalVerticalDrop = (0.5465 * gravity * Time * Time);
				//return TotalVerticalDrop * 10;
			}
			else if (distance > 56.f) {
				double TotalVerticalDrop = (0.5465 * gravity * Time * Time);
				//return TotalVerticalDrop * 10;
			}
			else if (distance > 78.f) {
				double TotalVerticalDrop = (0.56 * gravity * Time * Time);
				//return TotalVerticalDrop * 10;
			}

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -778367295) 
		{
			double TotalVerticalDrop = 1.000000 * 0.600000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1965232394) 
		{

			double TotalVerticalDrop = 0.750000 * 0.005000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1588298435)
		{
			double TotalVerticalDrop = 1.000000 * 0.600000;

		}

		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -852563019) 
		{

			double TotalVerticalDrop = 1.000000 * 0.700000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == 1103488722)
		{
			if (distance < 100.f) {
				double TotalVerticalDrop = (0.58 * gravity * Time * Time);
				return TotalVerticalDrop * 10;
			}

		}
		
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -1758372725)
		{
			double TotalVerticalDrop = 1.000000 * 0.700000;

		}
		else if (LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo() == -2069578888) 
		{
			double TotalVerticalDrop = 1.000000 * 0.600000;
		}

		if (distance < 50.f)
		{
			double TotalVerticalDrop = (0.451f * gravity * Time * Time);
			return TotalVerticalDrop * 10;
		}

		if (distance < 100.f)
		{
			double TotalVerticalDrop = (0.460f * gravity * Time * Time);
			return TotalVerticalDrop * 10;
		}
		if (distance >= 100.f)
		{
			double TotalVerticalDrop = (0.510f * gravity * Time * Time);
			return TotalVerticalDrop * 10;
		}


		if (distance >= 150.f)
		{
			double TotalVerticalDrop = (0.620f * gravity * Time);
			return TotalVerticalDrop * 10;
		}

		if (distance >= 200.f)
		{
			double TotalVerticalDrop = (0.672f * gravity * Time);
			return TotalVerticalDrop * 10;
		}
		else

			return 0;



	}
	

	void Prediction(Vector3 local, Vector3& target, Vector3 targetvel, float bulletspeed, float gravity) {
		float Dist = Math::Distance_3D(target, local);
		float BulletTime = Dist / bulletspeed;
		Vector3 vel = Vector3(targetvel.x, 0, targetvel.z) * 0.9f;
		Vector3 PredictVel = vel * BulletTime;
		target += PredictVel;
		double height = target.y - local.y;
		Vector3 dir = target - local;
		float DepthPlayerTarget = Vector3::my_sqrt((dir.x * dir.x) + (dir.z * dir.z));
		float drop = CalcBulletDrop(height, DepthPlayerTarget, bulletspeed, gravity);
		target.y += drop;
	}
	Vector3 get_aim_point(float speed, float gravity) {
		Vector3 ret = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);
		Prediction(LocalPlayer::Entity()->get_bone_pos(head), ret, reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->newVelocity(), speed, gravity);
		return ret;
	}
}
float GetFov(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!utils::w2s(Entity->get_bone_pos(Bone), ScreenPos)) return 1000.f;
	return Math::Distance_2D(Vector2(vars::stuff::ScreenWidth / 2, vars::stuff::ScreenHeight / 2), ScreenPos);
}
float GetFovHeli(Vector3 pos) {
	Vector2 ScreenPos;
	if (!utils::w2s(pos, ScreenPos)) return 1000.f;
	return Math::Distance_2D(Vector2(vars::stuff::ScreenWidth / 2, vars::stuff::ScreenHeight / 2), ScreenPos);
}
float GetGravity(int ammoid) {
	switch (ammoid) {
	case 14241751:
		return 1.f;
	case -1234735557:
		return 0.75f;
	case 215754713:
		return 0.75f;
	case -1023065463:
		return 0.5f;
	case -2097376851:
		return 0.75f;
	case -1321651331:
		return 1.25f;
	default:
		return 1.f;
	}
}

float GetBulletSpeed() {
	Item* active = LocalPlayer::Entity()->GetActiveWeapon();
	Weapon tar = active->Info();
	int ammo = active->LoadedAmmo();
	if (vars::weapons::fast_bullets)
	{
		if ( GetAsyncKeyState(vars::keys::lv_key)) {

			if (LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -852563019 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1545779598 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1335497659 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1953903201 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1443579727 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1367281941)
			{
				if (ammo == 0) return vars::weapons::fast_bullets ? tar.ammo[0].speed * 0.8 + vars::stuff::testFloat : tar.ammo[0].speed + vars::stuff::testFloat;
				for (Ammo am : tar.ammo) {
					for (int id : am.id) {
						if (id == ammo) {
							return vars::weapons::fast_bullets ? am.speed * 0.8 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
						}
					}
					if (am.id[0] == 0) return vars::weapons::fast_bullets ? am.speed * 0.8 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
				}
				return vars::weapons::fast_bullets ? 250.f * 0.8 + vars::stuff::testFloat : 250.f + vars::stuff::testFloat;
			}
			else
			{
				if (ammo == 0) return vars::weapons::fast_bullets ? tar.ammo[0].speed * 0.510 + vars::stuff::testFloat : tar.ammo[0].speed + vars::stuff::testFloat;
				for (Ammo am : tar.ammo) {
					for (int id : am.id) {
						if (id == ammo) {
							return vars::weapons::fast_bullets ? am.speed * 0.510 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
						}
					}
					if (am.id[0] == 0) return vars::weapons::fast_bullets ? am.speed * 0.510 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
				}
				return vars::weapons::fast_bullets ? 250.f * 0.510 + vars::stuff::testFloat : 250.f + vars::stuff::testFloat;
			}
		}
		else
		{
			if (ammo == 0) return vars::weapons::fast_bullets ? tar.ammo[0].speed * vars::weapons::bulletspeed + vars::stuff::testFloat : tar.ammo[0].speed + vars::stuff::testFloat;
			for (Ammo am : tar.ammo) {
				for (int id : am.id) {
					if (id == ammo) {
						return vars::weapons::fast_bullets ? am.speed * vars::weapons::bulletspeed + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
					}
				}
				if (am.id[0] == 0) return vars::weapons::fast_bullets ? am.speed * vars::weapons::bulletspeed + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
			}
			return vars::weapons::fast_bullets ? 250.f * vars::weapons::bulletspeed + vars::stuff::testFloat : 250.f + vars::stuff::testFloat;
		}
	}
	else
	{
		if (ammo == 0) return vars::weapons::fast_bullets ? tar.ammo[0].speed * 1.00 + vars::stuff::testFloat : tar.ammo[0].speed + vars::stuff::testFloat;
		for (Ammo am : tar.ammo) {
			for (int id : am.id) {
				if (id == ammo) {
					return vars::weapons::fast_bullets ? am.speed * 1.00 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
				}
			}
			if (am.id[0] == 0) return vars::weapons::fast_bullets ? am.speed * 1.00 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
		}
		return vars::weapons::fast_bullets ? 250.f * 1.00 + vars::stuff::testFloat : 250.f + vars::stuff::testFloat;
	}
}



void StepConstant(Vector2& angles) {
	bool smooth = vars::combat::smooth;
	Vector2 angles_step = angles - LocalPlayer::Entity()->viewangles();
	Math::Normalize(angles_step.x, angles_step.y);

	if (smooth) {
		float factor_pitch = (vars::combat::smooth_factor / 10.f);
		if (angles_step.x < 0.f) {
			if (factor_pitch > std::abs(angles_step.x)) {
				factor_pitch = std::abs(angles_step.x);
			}
			angles.x = LocalPlayer::Entity()->viewangles().x - factor_pitch;
		}
		else {
			if (factor_pitch > angles_step.x) {
				factor_pitch = angles_step.x;
			}
			angles.x = LocalPlayer::Entity()->viewangles().x + factor_pitch;
		}
	}
	if (smooth) {
		float factor_yaw = (vars::combat::smooth_factor / 10.f);
		if (angles_step.y < 0.f) {
			if (factor_yaw > std::abs(angles_step.y)) {
				factor_yaw = std::abs(angles_step.y);
			}
			angles.y = LocalPlayer::Entity()->viewangles().y - factor_yaw;
		}
		else {
			if (factor_yaw > angles_step.y) {
				factor_yaw = angles_step.y;
			}
			angles.y = LocalPlayer::Entity()->viewangles().y + factor_yaw;
		}
	}
}

void do_aimbot(BasePlayer* player) {
	if (!LocalPlayer::Entity() || !player) { return; }
	if (!LocalPlayer::Entity()->GetActiveWeapon() || player->IsDestroyed()) { return; }
	bool long_neck = vars::misc::long_neck && GetAsyncKeyState(vars::keys::longneck);
	Vector3 local = long_neck ? LocalPlayer::Entity()->get_bone_pos(head) + Vector3(0, 1.15, 0) : LocalPlayer::Entity()->get_bone_pos(head);
	Vector3 target = vars::combat::bodyaim ? player->get_bone_pos(spine1) : player->get_bone_pos(head);
	a::Prediction(local, target, player->newVelocity(), GetBulletSpeed(), GetGravity(LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo()));
	Vector2 Offset = Math::CalcAngle(local, target) - LocalPlayer::Entity()->viewangles();
	Math::Normalize(Offset.y, Offset.x);
	Vector2 AngleToAim = LocalPlayer::Entity()->viewangles() + Offset;
	if (vars::combat::smooth) {
		StepConstant(AngleToAim);
	}
	Math::Normalize(AngleToAim.y, AngleToAim.x);
	LocalPlayer::Entity()->set_viewangles(AngleToAim);
}
