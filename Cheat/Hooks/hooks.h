#include "defs.h"
#include <new>
#include "../Misc/PASTOR.H"

Vector3 last_head_pos = Vector3::Zero();
Vector3 last_neck_pos = Vector3::Zero();
Vector3 last_spine4_pos = Vector3::Zero();
Vector3 last_spine3_pos = Vector3::Zero();
Vector3 last_spine2_pos = Vector3::Zero();
Vector3 last_spine1_pos = Vector3::Zero();
Vector3 last_l_upperarm_pos = Vector3::Zero();
Vector3 last_l_forearm_pos = Vector3::Zero();
Vector3 last_l_hand_pos = Vector3::Zero();
Vector3 last_r_upperarm_pos = Vector3::Zero();
Vector3 last_r_forearm_pos = Vector3::Zero();
Vector3 last_r_hand_pos = Vector3::Zero();
Vector3 last_pelvis_pos = Vector3::Zero();
Vector3 last_l_knee_pos = Vector3::Zero();
Vector3 last_l_foot_pos = Vector3::Zero();
Vector3 last_r_knee_pos = Vector3::Zero();
Vector3 last_r_foot_pos = Vector3::Zero();

bool magic_projectile_test = false;
float insta_travel_check_time = -1.f;
float lastshottime;
bool is_noclipping = false;

int flick = 0;
int yaw = 100;

bool targeted = false;
float target_time = 0.f;

#define minm( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#define maxx( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )

Vector3 GetTrajectoryPoint(Vector3& startingPosition, Vector3 initialVelocity, float timestep, float gravity)
{
	float physicsTimestep = Time::fixedDeltaTime();
	Vector3 stepVelocity = initialVelocity * physicsTimestep;

	//Gravity is already in meters per second, so we need meters per second per second
	Vector3 stepGravity(0, physicsTimestep * physicsTimestep * gravity, 0);

	return startingPosition + (stepVelocity * timestep) + ((stepGravity * (timestep * timestep + timestep)) / 2.0f);;
}

using namespace hk_defs;

auto gameAssembly = GetModuleHandleA(("GameAssembly.dll"));

#define ProcAddr(func) GetProcAddress(gameAssembly, func)

template<typename T, typename... Args>
inline T call(const char* func, Args... args) {
	return reinterpret_cast<T(__fastcall*)(Args...)>(ProcAddr(func))(args...);
}
namespace System {
	class Object_ {
	public:

	};
	template<typename T = void*>
	class Array {
	public:
		uint32_t size() {
			if (!this) return 0;
			return *reinterpret_cast<uint32_t*>(this + 0x18);
		}
		T get(int idx) {
			if (!this) return T{};
			return *reinterpret_cast<T*>(this + (0x20 + (idx * 0x8)));
		}
		void add(int idx, T value) {
			if (!this) return;
			*reinterpret_cast<T*>(this + (0x20 + (idx * 0x8))) = value;
		}
	};
	class String : public Object_ {
	public:
		char pad_0000[0x10];
		int len;
		wchar_t buffer[0];

		static String* New(const char* str) {
			return call<String*, const char*>(("il2cpp_string_new"), str);
		}
	};
}
class MonoBehaviour {
public:
	static inline System::Object_* (*StartCoroutine_)(MonoBehaviour*, System::Object_*) = nullptr;
	System::Object_* StartCoroutine(System::Object_* routine) {
		return StartCoroutine_(this, routine);
	}
};
uintptr_t shader;


namespace BulletTP {
	void SimulateProjectile(Vector3 position, Vector3 velocity, float partialTime, float travelTime, Vector3 gravity, float drag, Vector3 prevPosition, Vector3 prevVelocity)
	{
		float num = 0.03125f;
		prevPosition = position;
		prevVelocity = velocity;
		if (partialTime > 0)
		{
			float num2 = num - partialTime;
			if (travelTime < num2)
			{
				prevPosition = position;
				prevVelocity = velocity;
				position += velocity * travelTime;
				partialTime += travelTime;
				return;
			}
			prevPosition = position;
			prevVelocity = velocity;
			position += velocity * num2;
			velocity += gravity * num;
			velocity -= velocity * (drag * num);
			travelTime -= num2;
		}
		int num3 = int(travelTime / num);
		for (int i = 0; i < num3; i++)
		{
			prevPosition = position;
			prevVelocity = velocity;
			position += velocity * num;
			velocity += gravity * num;
			velocity -= velocity * (drag * num);
		}
		partialTime = travelTime - num * (float)num3;
		if (partialTime > 0)
		{
			prevPosition = position;
			prevVelocity = velocity;
			position += velocity * partialTime;
		}
	}

	bool BulletTP(Projectile* instance, Vector3 NextCurrentPosition, Vector3 CurrentPosition, Vector3 CurrentVelocity, float deltaTime)
	{
		Vector3 aa; Vector3 bb;

		BasePlayer* targetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
		auto playerProjectileUpdate = (PlayerProjectileUpdate*)methods::object_new(il2cpp::_init_class(("PlayerProjectileUpdate"), ("ProtoBuf")));

		if (instance->projectileID() == 0)
			return false;

		if (instance->integrity() <= 0.f)
			return false;

		if (!targetPlayer)
			return false;

		auto Line = NextCurrentPosition - CurrentPosition;
		auto LineLength = Line.UnityMagnitude();
		Line.UnityNormalize();

		auto CenterPosition = targetPlayer->PivotPoint() + Vector3(0.f, 0.9f, 0.f);
		auto v = CenterPosition - CurrentPosition;
		auto d = v.DotSero(Line);

		if (d < 0.0f)
		{
			d = 0.0f;
		}
		else if (d > LineLength)
		{
			d = LineLength;
		}

		auto OriginalClosestPointOnLine = CurrentPosition + Line * d;
		auto ClosestPointOnLine = OriginalClosestPointOnLine;

		auto ClosestPoint = utils::ClosestPoint(targetPlayer, ClosestPointOnLine);
		auto OriginalDistance = targetPlayer->Distance(ClosestPointOnLine);
		auto Distance = OriginalDistance;

		Vector3 HitPointWorld = ClosestPoint;

		if (Distance > 2.59f)
		{
			return false;
		}

		if (Distance > 1.2f)
		{
			auto endPositionTrajectoryUsage = Distance - 1.2f + 0.01f;
			auto amountNeeded = endPositionTrajectoryUsage / Distance;
			auto direction = HitPointWorld - ClosestPointOnLine;
			auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

			if (ClosestPointOnLine.Distance(newPosition) > 1.f || !GamePhysics::LineOfSight(ClosestPointOnLine, newPosition, 10551296, 0.f))
				return false;

			ClosestPointOnLine = newPosition;
			Distance = targetPlayer->Distance(ClosestPointOnLine);
		}

		if (Distance > 1.f)
		{
			auto playerDistanceUsage = minm(0.19f, Distance - 1.f);
			auto amountNeeded = 1.f - playerDistanceUsage / Distance;
			auto direction = HitPointWorld - ClosestPointOnLine;
			auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

			HitPointWorld = newPosition;
		}

		if (!GamePhysics::LineOfSight(ClosestPointOnLine, HitPointWorld, 10551296, 0.f) || !GamePhysics::LineOfSight(CurrentPosition, OriginalClosestPointOnLine, 10551296, 0.f) ||
			!GamePhysics::LineOfSight(OriginalClosestPointOnLine, ClosestPointOnLine, 10551296, 0.f) || !GamePhysics::LineOfSight(CenterPosition, HitPointWorld, 10551296, 0.f))
		{
			return false;
		}

		auto LPlayer = LocalPlayer::Entity();

		instance->previousPosition() = instance->currentPosition();
		instance->currentPosition() = OriginalClosestPointOnLine;
		instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
		instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

		if (playerProjectileUpdate) {
			((PlayerProjectileUpdate*)playerProjectileUpdate)->projectileID() = instance->projectileID();
			((PlayerProjectileUpdate*)playerProjectileUpdate)->travelTime() = instance->traveledTime();
			((PlayerProjectileUpdate*)playerProjectileUpdate)->curVelocity() = instance->currentVelocity();
			((PlayerProjectileUpdate*)playerProjectileUpdate)->curPosition() = instance->currentPosition();

			LPlayer->SPU(playerProjectileUpdate);
		}

		instance->previousPosition() = instance->currentPosition();
		instance->currentPosition() = ClosestPointOnLine;
		instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
		instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

		if (playerProjectileUpdate) {
			((PlayerProjectileUpdate*)playerProjectileUpdate)->projectileID() = instance->projectileID();
			((PlayerProjectileUpdate*)playerProjectileUpdate)->travelTime() = instance->traveledTime();
			((PlayerProjectileUpdate*)playerProjectileUpdate)->curVelocity() = instance->currentVelocity();
			((PlayerProjectileUpdate*)playerProjectileUpdate)->curPosition() = instance->currentPosition();

			LPlayer->SPU(playerProjectileUpdate);
		}

		HitTest* hTest = instance->hitTest();

		if (HitPointWorld == instance->currentPosition())
		{
			HitPointWorld.y += 0.01f;
		}

		Ray ray = Ray(instance->currentPosition(), (HitPointWorld - instance->currentPosition()).Normalized());

		SimulateProjectile(instance->currentPosition(), instance->currentVelocity(), instance->tumbleSpeed(), maxx(instance->traveledTime() - instance->closeFlybyDistance(), 0), Physics::get_gravity() * instance->gravityModifier(), instance->drag(), aa, bb);

		hTest->AttackRay() = ray;
		hTest->DidHit() = true;
		hTest->HitTransform() = targetPlayer->transform_(head);
		hTest->HitMaterial() = il2cpp::String::New(("Flesh"));
		hTest->HitEntity() = targetPlayer;
		hTest->HitPoint() = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->transform_(head)->InverseTransformPoint(HitPointWorld);
		hTest->HitNormal() = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->transform_(head)->InverseTransformPoint(HitPointWorld);
		hTest->damageProperties() = instance->damageProperties();

		instance->DoHit(hTest, HitPointWorld, hTest->HitNormalWorld());
		instance->Update();

		return true;
	}

	bool STW(Projectile* instance, Vector3 NextCurrentPosition, Vector3 CurrentPosition, Vector3 CurrentVelocity, float deltaTime)
	{
		while (true) {
			Vector3 aa; Vector3 bb;

			BasePlayer* targetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
			auto playerProjectileUpdate = (PlayerProjectileUpdate*)methods::object_new(il2cpp::_init_class(("PlayerProjectileUpdate"), ("ProtoBuf")));

			if (instance->projectileID() == 0)
				return false;

			if (instance->integrity() <= 0.f)
				return false;

			if (!targetPlayer)
				return false;

			auto Line = NextCurrentPosition - CurrentPosition;
			auto LineLength = Line.UnityMagnitude();
			Line.UnityNormalize();

			auto CenterPosition = targetPlayer->PivotPoint() + Vector3(0.f, 0.9f, 0.f);
			auto v = CenterPosition - CurrentPosition;
			auto d = v.DotSero(Line);

			if (d < 0.0f)
			{
				d = 0.0f;
			}
			else if (d > LineLength)
			{
				d = LineLength;
			}

			auto OriginalClosestPointOnLine = CurrentPosition + Line * d;
			auto ClosestPointOnLine = OriginalClosestPointOnLine;

			auto ClosestPoint = utils::ClosestPoint(targetPlayer, ClosestPointOnLine);
			auto OriginalDistance = targetPlayer->Distance(ClosestPointOnLine);
			auto Distance = OriginalDistance;

			Vector3 traveledThisUpdate = instance->currentVelocity() * deltaTime;

			Vector3 HitPointWorld = ClosestPoint;

			RaycastHit hitInfo;
			if (!GamePhysics::Trace(Ray(instance->currentPosition(), instance->currentVelocity().Normalized()), 0.f, &hitInfo, traveledThisUpdate.Length() + 1.f, LayerMasks::Terrain | LayerMasks::Construction | LayerMasks::World))
			{
				break;
			}

			if (Distance > 1.2f)
			{
				auto endPositionTrajectoryUsage = Distance - 1.2f + 0.01f;
				auto amountNeeded = endPositionTrajectoryUsage / Distance;
				auto direction = HitPointWorld - ClosestPointOnLine;
				auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

				if (ClosestPointOnLine.Distance(newPosition) > 1.f || !GamePhysics::LineOfSight(ClosestPointOnLine, newPosition, 10551296, 0.f))
					return false;

				ClosestPointOnLine = newPosition;
				Distance = targetPlayer->Distance(ClosestPointOnLine);
			}

			if (Distance > 1.f)
			{
				auto playerDistanceUsage = minm(0.19f, Distance - 1.f);
				auto amountNeeded = 1.f - playerDistanceUsage / Distance;
				auto direction = HitPointWorld - ClosestPointOnLine;
				auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

				HitPointWorld = newPosition;
			}

			auto LPlayer = LocalPlayer::Entity();

			instance->previousPosition() = instance->currentPosition();
			instance->currentPosition() = OriginalClosestPointOnLine;
			instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
			instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

			if (playerProjectileUpdate) {
				((PlayerProjectileUpdate*)playerProjectileUpdate)->projectileID() = instance->projectileID();
				((PlayerProjectileUpdate*)playerProjectileUpdate)->travelTime() = instance->traveledTime();
				((PlayerProjectileUpdate*)playerProjectileUpdate)->curVelocity() = instance->currentVelocity();
				((PlayerProjectileUpdate*)playerProjectileUpdate)->curPosition() = instance->currentPosition();

				LPlayer->SPU(playerProjectileUpdate);
			}

			instance->previousPosition() = instance->currentPosition();
			instance->currentPosition() = ClosestPointOnLine;
			instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
			instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

			if (playerProjectileUpdate) {
				((PlayerProjectileUpdate*)playerProjectileUpdate)->projectileID() = instance->projectileID();
				((PlayerProjectileUpdate*)playerProjectileUpdate)->travelTime() = instance->traveledTime();
				((PlayerProjectileUpdate*)playerProjectileUpdate)->curVelocity() = instance->currentVelocity();
				((PlayerProjectileUpdate*)playerProjectileUpdate)->curPosition() = instance->currentPosition();

				LPlayer->SPU(playerProjectileUpdate);
			}

			HitTest* hTest = instance->hitTest();

			if (HitPointWorld == instance->currentPosition())
			{
				HitPointWorld.y += 0.01f;
			}

			Ray ray = Ray(instance->currentPosition(), (HitPointWorld - instance->currentPosition()).Normalized());

			SimulateProjectile(instance->currentPosition(), instance->currentVelocity(), instance->tumbleSpeed(), maxx(instance->traveledTime() - instance->closeFlybyDistance(), 0), Physics::get_gravity() * instance->gravityModifier(), instance->drag(), aa, bb);

			hTest->AttackRay().origin = hitInfo.point;
			hTest->AttackRay().dir = instance->currentVelocity().Normalized().Normalized();
			hTest->DidHit() = true;
			hTest->HitTransform() = targetPlayer->transform_(head);
			hTest->HitMaterial() = il2cpp::String::New(("Flesh"));
			hTest->HitEntity() = targetPlayer;
			hTest->HitPoint() = hTest->HitTransform()->InverseTransformPoint(HitPointWorld); // tut pipec
			hTest->HitNormal() = hTest->HitTransform()->InverseTransformPoint(HitPointWorld);
			hTest->damageProperties() = instance->damageProperties();

			instance->DoHit(hTest, HitPointWorld, hTest->HitNormalWorld());
			instance->Update();

			return true;
		}
	}
}

namespace hk {
	namespace exploit {

		void DoMovement(Projectile* projectile, float deltaTime) {
			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			BaseProjectile* _held = held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			
			if (vars::combat::bullet_tp) {
				float dt;

				if (projectile->projectileID() == 0)
					return;

				if (projectile->integrity() <= 0.f)
					return;

				auto* target_player = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);

				Ray line = Ray(projectile->currentPosition(), projectile->currentPosition() + (projectile->currentVelocity() * dt));
				Vector3 corrected_position = line.ClosestPoint(target_player->transform_(head)->position());
				Vector3 velocity_per_tick = projectile->currentVelocity() * dt;

				auto transform = reinterpret_cast<BasePlayer*>(target_player)->transform_(head);
				auto target_pos = transform->position();
				auto proj_pos = projectile->transform()->position();
				auto dist = target_pos.distance(proj_pos);

				PlayerProjectileUpdate* player_update = (PlayerProjectileUpdate*)methods::object_new(il2cpp::_init_class(("PlayerProjectileUpdate"), ("ProtoBuf")));

				float velocity_per_tick_speed = velocity_per_tick.Length();
				float speed_fraction = 1.f / velocity_per_tick_speed;
				float distance_traveled = (corrected_position - projectile->currentPosition()).Length();
				float traveled_time = distance_traveled * speed_fraction * dt;

				Vector3 aa;
				Vector3 bb;

				Vector3 gr = Physics::get_gravity();
				float travel = maxx(projectile->traveledTime() - projectile->closeFlybyDistance(), 0);

				Ray ray = Ray(projectile->currentPosition(), (transform->position().Vector3::move_towards(corrected_position, target_pos, 1.0f) - projectile->currentPosition()).UnityNormalize());

				if (transform) {
					auto dist = target_pos.distance(projectile->transform()->position());
					if (dist < 2.00f) {
						const auto hit_test = projectile->hitTest();
						auto newpos = Vector3::move_towards(projectile->transform()->position(), target_pos, 1.0f);
						Ray r(projectile->transform()->position(), newpos);
						player_update->projectileID() = projectile->projectileID();
						player_update->travelTime() = projectile->traveledTime();
						player_update->curVelocity() = projectile->currentVelocity();
						player_update->curPosition() = projectile->currentPosition();
						LocalPlayer::Entity()->SPU(player_update);
						projectile->previousPosition() = projectile->currentPosition();
						projectile->traveledDistance() += distance_traveled;
						projectile->traveledTime() += traveled_time;
						projectile->sentPosition() = corrected_position;
						player_update->projectileID() = projectile->projectileID();
						player_update->travelTime() = projectile->traveledTime();
						player_update->curVelocity() = projectile->currentVelocity();
						player_update->curPosition() = corrected_position;
						LocalPlayer::Entity()->SPU(player_update);
						player_update->curPosition() = newpos;
						LocalPlayer::Entity()->SPU(player_update);
						hit_test->MaxDistance() = 1000.0f;
						hit_test->AttackRay().origin = corrected_position;
						hit_test->AttackRay().dir = (transform->position() - corrected_position).normalized();
						hit_test->DidHit() = true;
						hit_test->HitTransform() = transform;
						hit_test->HitEntity() = target_player;
						hit_test->HitPoint() = hit_test->HitTransform()->InverseTransformPoint(transform->position().Vector3::move_towards(corrected_position, target_pos, 1.0f));
						hit_test->HitMaterial() = il2cpp::String::New(("Flesh"));
						BulletTP::SimulateProjectile(projectile->currentPosition(), projectile->currentVelocity(), projectile->tumbleSpeed(), travel, gr * projectile->gravityModifier(), projectile->drag(), aa, bb);
						projectile->DoHit(hit_test, newpos, Vector3::Zero());
					}
				}
			}

			if (vars::combat::stw) {
				Vector3 a = projectile->currentVelocity() * deltaTime;
				float magnitude = a.Length();
				float num2 = 1 / magnitude;
				Vector3 vec2 = a * num2;
				Vector3 vec3 = projectile->currentPosition() + vec2 * magnitude;

				BulletTP::STW(projectile, vec3, projectile->currentPosition(), projectile->currentVelocity(), deltaTime);
			}


			BasePlayer* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
			auto transform = reinterpret_cast<BasePlayer*>(TargetPlayer)->get_bone_pos(head);

			static float r = 1.00f, g = 0.00f, b = 1.00f;
			static int cases = 0;
			switch (cases) {
			case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
			case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
			case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
			case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
			default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
			}

			auto currentVelocity = projectile->currentVelocity() * deltaTime;
			auto intmag = currentVelocity.Magnitude();
			auto intnum = 1.f / intmag;
			auto speed = currentVelocity * intnum;
			auto lineposition = projectile->currentPosition() + speed * currentVelocity.Magnitude();

			if (projectile->isAuthoritative() && vars::misc::bullet_tracers) {

				DDraw::Line(projectile->currentPosition(), lineposition, Color(r, g, b, g), 0.9f, false, false);
			}

			if (projectile->isAuthoritative() && vars::combat::always_heli_rotor) {
				f_object target = f_object::get_closest_object(projectile->currentPosition(), ("xdwadawfgaga"),
					Vector3::Zero(), Vector3::Zero(), Vector3::Zero(),
					true, ("BaseHelicopter"));

				if (target.valid) {
					Vector3 tar = reinterpret_cast<BaseEntity*>(target.entity)->transform()->position();
					if (utils::LineOfSight(tar, projectile->currentPosition()) && Math::Distance_3D(projectile->currentPosition(), tar) < 35.0f) {
						if (projectile->traveledDistance() > 35.0f) {
							Transform* transform = reinterpret_cast<BaseEntity*>(target.entity)->transform();

							HitTest* hitTest = projectile->hitTest();
							hitTest->DidHit() = true;
							hitTest->HitEntityt((BaseEntity*)target.entity);
							hitTest->HitTransform() = transform;
							hitTest->HitPoint() = transform->InverseTransformPoint(projectile->currentPosition());
							hitTest->HitNormal() = transform->InverseTransformDirection(projectile->currentPosition());
							hitTest->AttackRay() = Ray(projectile->currentPosition(), tar - projectile->currentPosition());
							projectile->DoHit(hitTest, hitTest->HitPointWorld(), hitTest->HitNormalWorld());
							return;
						}
					}
				}
			}

			auto mod = projectile->mod();
			auto ammo_type = projectile->ammoType();

			bool fast = false;
			switch (ammo_type)
			{
			case 785728077:
				fast = true;
				break;
			case -1691396643:
				fast = true;
				break;
			case 51984655:
				fast = true;
				break;
			case -1211166256:
				fast = true;
				break;
			case 1712070256:
				fast = true;
				break;
			case 605467368:
				fast = true;
				break;
			case -1321651331:
				fast = true;
				break;
			case -1685290200:
				fast = true;
				break;
			case -727717969:
				fast = true;
				break;
			case -1036635990:
				fast = true;
				break;
			case 588596902:
				fast = true;
				break;
			}

			Vector3 l_current_pos = projectile->currentPosition();

			return original_domovement(projectile, deltaTime);
		}
	}
	

		
	namespace misc {
		void VisUpdateUsingCulling(BasePlayer* pl, float dist, bool vis) {
			if (vars::players::chams) {
				return original_UnregisterFromVisibility(pl, 2.f, true);
			}
			else {
				return original_UnregisterFromVisibility(pl, dist, vis);
			}
		}
		float LastUpdate = 0.f;
		GameObject* CreateEffect(pUncStr strPrefab, Effect* effect) {
			auto effectName = strPrefab->str;
			auto position = read(effect + 0x5C, Vector3);
			if (vars::visuals::raid_esp && effect && strPrefab->str && !position.empty()) {
				switch (RUNTIME_CRC32_W(effectName)) {
				case STATIC_CRC32("assets/prefabs/tools/c4/effects/c4_explosion.prefab"):
					LogSystem::LogExplosion(C4, position);
					//LogSystem::Log(StringFormat::format((L"✚ Explosion: %ls, Distsnce: %1.0fm"), wC4.c_str( ), Math::Distance_3D(LocalPlayer::Entity( )->get_bone_pos(head), position)), 15.f);
					break;
				case STATIC_CRC32("assets/prefabs/weapons/satchelcharge/effects/satchel-charge-explosion.prefab"):
					LogSystem::LogExplosion(Satchel, position);
							//LogSystem::Log(StringFormat::format((L"✚ Explosion: %ls, Distsnce: %1.0fm"), wSatchel.c_str( ), Math::Distance_3D(LocalPlayer::Entity( )->get_bone_pos(head), position)), 15.f);
					break;
				case STATIC_CRC32("assets/prefabs/weapons/rocketlauncher/effects/rocket_explosion_incendiary.prefab"):
					LogSystem::LogExplosion(IncenRocket, position);
					//LogSystem::Log(StringFormat::format((L"✚ Explosion: %ls, Distsnce: %1.0fm"), wIncenRocket.c_str( ), Math::Distance_3D(LocalPlayer::Entity( )->get_bone_pos(head), position)), 15.f);
					break;
				case STATIC_CRC32("assets/prefabs/weapons/rocketlauncher/effects/rocket_explosion.prefab"):
					LogSystem::LogExplosion(Rocket, position);
					//LogSystem::Log(StringFormat::format((L"✚ Explosion: %ls, Distsnce: %1.0fm"), wRocket.c_str( ), Math::Distance_3D(LocalPlayer::Entity( )->get_bone_pos(head), position)), 15.f);
					break;
				}
			}
			return original_createeffect(strPrefab, effect);
		}

		void movementstop(BasePlayer* LocalPlayer)
		{
			const auto last_sent_tick = LocalPlayer->lastSentTick();
			const auto player_movement = LocalPlayer->movement();
			auto LastSentTickPos = last_sent_tick->__position();
			Vector3 z = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);
			
			player_movement->TeleportTo(Vector3(z.x, z.y - 0.001, z.z));
	
		}

		void __fastcall SetFlying(ModelState* a1, bool a2) { }
		void SendClientTick(BasePlayer* baseplayer) {
			if (!baseplayer) return original_sendclienttick(baseplayer);
			if (vars::misc::anti_aim) {
				if (Time::fixedTime() > (target_time + 0.1f) && target_time != 0.0f && targeted)
				{
					target_time = 0.0f;
					targeted = false;
				}
				auto input = read(baseplayer + O::BasePlayer::input, uintptr_t);
				auto state = read(input + 0x20, uintptr_t);
				auto current = read(state + 0x10, uintptr_t);
				
				Vector3 real_angles = safe_read(current + 0x18, Vector3);
				Vector3 spin_angles = Vector3::Zero();
				if (vars::misc::anti_aim_yaw == 0) { //x = yaw (up/down), y = pitch (spin), z = roll?????;
					if (targeted)
						spin_angles = Vector3(-999.f, (rand() % 999 + -999), (rand() % 999 + -999));
				}
				if (vars::misc::anti_aim_yaw == 1) { //backwards
					spin_angles.y = real_angles.y + (targeted ? (rand() % -180 + 1) : 180.f);
				}
				if (vars::misc::anti_aim_yaw == 2) { //backwards (down)
					spin_angles.x = (targeted ? 999.f : -999.f);
					spin_angles.z = 0.f;
					spin_angles.y = real_angles.y + 180.f;
				}
				if (vars::misc::anti_aim_yaw == 3) { //backwards (up)
					spin_angles.x = (targeted ? -999.f : 999.f);
					spin_angles.z = (targeted ? -999.f : 999.f);
					spin_angles.y = real_angles.y + 180.f;
				}
				if (vars::misc::anti_aim_yaw == 4) { //left
					spin_angles.y = real_angles.y + (targeted ? (rand() % -90 + 1) : 90.f);
				}
				if (vars::misc::anti_aim_yaw == 5) { //left (down)
					spin_angles.x = (targeted ? 999.f : -999.f);
					spin_angles.z = 0.f;
					spin_angles.y = real_angles.y + (targeted ? (rand() % -90 + 1) : 90.f);
				}
				if (vars::misc::anti_aim_yaw == 6) { //left (up)
					spin_angles.x = (targeted ? -999.f : 999.f);
					spin_angles.z = (targeted ? -999.f : 999.f);
					spin_angles.y = real_angles.y + (targeted ? (rand() % -90 + 1) : 90.f);
				}
				if (vars::misc::anti_aim_yaw == 7) { //right
					spin_angles.y = real_angles.y + (targeted ? (rand() % 90 + 1) : -90.f);
				}
				if (vars::misc::anti_aim_yaw == 8) { //right (down)
					spin_angles.x = (targeted ? 999.f : -999.f);
					spin_angles.z = 0.f;
					spin_angles.y = real_angles.y + (targeted ? (rand() % 90 + 1) : -90.f);
				}
				if (vars::misc::anti_aim_yaw == 9) { //right (up)
					spin_angles.x = (targeted ? -999.f : 999.f);
					spin_angles.z = (targeted ? -999.f : 999.f);
					spin_angles.y = real_angles.y + (targeted ? (rand() % 90 + 1) : -90.f);
				}
				if (vars::misc::anti_aim_yaw == 10) { //jitter
					if (jitter <= jitter_speed * 1)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : -45.f);
					}
					else if (jitter <= jitter_speed * 2)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : 45.f);
					}
					else if (jitter <= jitter_speed * 3)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % 180 + 1) : -180.f);
						jitter = 1;
					}
					jitter = jitter + 1;
					spin_angles.y = real_angles.y;
				}
				if (vars::misc::anti_aim_yaw == 11) { //jitter (down)
					if (jitter <= jitter_speed * 1)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : -45.f);
					}
					else if (jitter <= jitter_speed * 2)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % -45 + 1) : 45.f);
					}
					else if (jitter <= jitter_speed * 3)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % -180 + 1) : 180.f);
						jitter = 1;
					}
					jitter = jitter + 1;
					spin_angles.x = (targeted ? (rand() % 999 + 1) : -999.f);
					spin_angles.z = 0.f;
					spin_angles.y = real_angles.y;
				}
				if (vars::misc::anti_aim_yaw == 12) { //jitter (up)
					if (jitter <= jitter_speed * 1)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % -45 + 1) : 45.f);
					}
					else if (jitter <= jitter_speed * 2)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : -45.f);
					}
					else if (jitter <= jitter_speed * 3)
					{
						spin_angles.y = real_angles.y + (targeted ? (rand() % 180 + 1) : -180.f);
						jitter = 1;
					}
					jitter = jitter + 1;
					spin_angles.x = (targeted ? -999.f : 999.f);
					spin_angles.z = (targeted ? -999.f : 999.f);
					spin_angles.y = real_angles.y;
				}
				if (vars::misc::anti_aim_yaw == 13) { //spin
					spin_angles.y = targeted ? -(real_angles.y + (spin_speed * spin++)) : real_angles.y + (spin_speed * spin++);
					if (spin > (360 / spin_speed))
						spin = 1;
				}
				if (vars::misc::anti_aim_yaw == 14) { //spin (down)
					spin_angles.x = (targeted ? 999.f : -999.f);
					spin_angles.z = 0.f;
					spin_angles.y = targeted ? -(real_angles.y + (spin_speed * spin++)) : real_angles.y + (spin_speed * spin++);
					if (spin > (360 / spin_speed))
						spin = 1;
				}
				if (vars::misc::anti_aim_yaw == 15) { //spin (up)
					spin_angles.x = (targeted ? -999.f : 999.f);
					spin_angles.y = targeted ? -(real_angles.y + (spin_speed * spin++)) : real_angles.y + (spin_speed * spin++);
					spin_angles.z = (targeted ? -999.f : 999.f);
					if (spin > (360 / spin_speed))
						spin = 1;
				}
				if (vars::misc::anti_aim_yaw == 16) { //random
					spin_angles = Vector3((rand() % 999 + -999), (rand() % 999 + -999), (rand() % 999 + -999));
				}

				if (spin_angles != Vector3::Zero())
					write(current + 0x18, spin_angles, Vector3);
			}
		
			if (baseplayer->userID() == LocalPlayer::Entity()->userID()) {
				Vector3 current = baseplayer->transform()->position();
				Vector3 old = baseplayer->lastSentTick()->tick_position();
				Vector3 vector4 = (baseplayer->transform()->position() - baseplayer->lastSentTick()->tick_position());
				Vector3 overrided = Vector3(current.x, current.y, current.z);
				if (vars::misc::fly_auto_stopper && VFlyhack >= (VMaxFlyhack - 1.5f))
				{
					overrided = Vector3(overrided.x, current.y < old.y ? (current.y - 0.3f) : old.y, overrided.z);
				}
				if (vars::misc::fly_auto_stopper && HFlyhack >= (HMaxFlyhack - 2.7f))
				{
					overrided = Vector3(old.x, overrided.y, old.z);
				}
				if (vars::misc::fly_auto_stopper && HFlyhack >= (HMaxFlyhack - 2.7f) ||
					vars::misc::fly_auto_stopper && VFlyhack >= (VMaxFlyhack - 1.5f))
				{
					if (overrided != current)
						baseplayer->movement()->TeleportTo(overrided);
				}

				if (vars::misc::Mautostop && GetAsyncKeyState(vars::keys::autostopperm)) {
					movementstop(baseplayer);
				}
				if (vars::misc::farmbot && vars::misc::jumpfarm) {
					static float b = 4.0f;
					if (b >= 4.0f) {
						float radius = baseplayer->GetRadius();
						float height = baseplayer->GetHeight(false);
						Vector3 vector = (baseplayer->lastSentTick()->tick_position() + baseplayer->transform()->position()) * 0.5f;
						float flyhack_extrusion = 0.25f;
						Vector3 vector2 = vector + Vector3(0.0f, radius - flyhack_extrusion, 0.0f);
						Vector3 vector3 = vector + Vector3(0.0f, height - radius, 0.0f);
						float radius2 = radius - 0.05f;
						bool isgrounded = GamePhysics::CheckCapsule(vector2, vector3, radius2, 1503731969, GamePhysics::QueryTriggerInteraction::Ignore);
						if (isgrounded) {
							baseplayer->movement()->Jump(baseplayer->modelState());
							b = 0.0f;
						}
					}
					b++;
				}
			}
			return original_sendclienttick(baseplayer);
		}
		void DoFixedUpdate(PlayerWalkMovement* movement, ModelState* modelstate, PlayerWalkMovement* self) {
			
			float speed = (read(movement + 0x143, bool) /*swimming*/ || read(movement + 0x50, float) /* ducked */ > 0.5f) ? 1.7f : (read(movement + 0x145, bool) /*jumping*/ ? 8.f : 5.5f);
			if (vars::misc::farmbot) {
				if (vars::misc::farmbot_trees) {
					Vector3 vel = read(movement + 0x34, Vector3);
					f_object closest = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head), ("tree"));
					if (closest.valid) {
						Vector3 direction = (closest.position - LocalPlayer::Entity()->get_bone_pos(head)).Normalized() * speed;
						write(movement + 0x34, Vector3(direction.x, vel.y, direction.z), Vector3);
					}
				}
				if (vars::misc::farmbot_ore) {
					Vector3 vel = read(movement + 0x34, Vector3);
					f_object closest = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head), ("ore.prefab"));
					if (closest.valid) {
						Vector3 direction = (closest.position - LocalPlayer::Entity()->get_bone_pos(head)).Normalized() * speed;
						write(movement + 0x34, Vector3(direction.x, vel.y, direction.z), Vector3);
					}
				}
				if (vars::misc::farmbot_barrels) {
					Vector3 vel = read(movement + 0x34, Vector3);
					f_object closest = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head), ("barrel"));
					if (closest.valid) {
						Vector3 direction = (closest.position - LocalPlayer::Entity()->get_bone_pos(head)).Normalized() * speed;
						write(movement + 0x34, Vector3(direction.x, vel.y, direction.z), Vector3);
					}
				}
			}

			original_dofixedupdate(movement, modelstate);
		}

		void HandChams() {
			if (!LocalPlayer::Entity()->is_alive()) return;
			if (vars::visuals::hand_chams) {
				static float r = 1.00f, g = 0.00f, b = 1.00f;
				static int cases = 0;
				switch (cases) {
				case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
				case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
				case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
				case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
				default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
				}
				auto model = get_activemodel();
				auto renderers = ((Networkable*)model)->GetComponentsInChildren(GetType(("UnityEngine"), ("Renderer")));
				if (renderers)
				{
					auto sz = *reinterpret_cast<int*>(renderers + 0x18);
					for (int i = 0; i < sz; i++) {
						auto renderer = *reinterpret_cast<Renderer_**>(renderers + 0x20 + i * 0x8);
						if (!renderer) continue;
						Material* material = renderer->material();
						if (!material) continue;

						if (vars::visuals::hand_chams_type == 0)
						{
							renderer->set_material(Gold);
						}
						else if (vars::visuals::hand_chams_type == 1)
						{
							renderer->set_material(Glitter);
						}
						else if (vars::visuals::hand_chams_type == 2)
						{
							renderer->set_material(gaga);
						}
						else if (vars::visuals::hand_chams_type == 3)
						{
							renderer->set_material(Pixel);
						}
						else if (vars::visuals::hand_chams_type == 4)
						{
							renderer->set_material(gagahga);
						}
						else if (vars::visuals::hand_chams_type == 5)
						{
							renderer->set_material(Ghost);
						}
						else if (vars::visuals::hand_chams_type == 6)
						{
							renderer->set_material(GredSpace);
						}

						else if (vars::visuals::hand_chams_type == 7)
						{
							renderer->set_material(Glitch2);
						}

						else if (vars::visuals::hand_chams_type == 8)
						{
							renderer->set_material(Glitch3);
						}

						else if (vars::visuals::hand_chams_type == 9)
						{
							renderer->set_material(Slick);
						}
							
						else if (vars::visuals::hand_chams_type == 10)
						{
							renderer->set_material(afagfgtqag);

						}

						else if (vars::visuals::hand_chams_type == 11)
						{
							renderer->set_material(Ghost2);

						}

						else if (vars::visuals::hand_chams_type == 12)
						{
							renderer->set_material(GredSpace);

						}

						else if (vars::visuals::hand_chams_type == 13)
						{
							renderer->set_material(GalaxyFire);

						}

						else if (vars::visuals::hand_chams_type == 14)
						{
							renderer->set_material(GalaxyNightFire);

						}

						else if (vars::visuals::hand_chams_type == 15)
						{
							renderer->set_material(JOPA);
								
						}

						else if (vars::visuals::hand_chams_type == 16)
						{
							renderer->set_material(Kakashke);


						}
					}
				}
			}
		}

		void ClientInput(BasePlayer* baseplayah, ModelState* ModelState) {
			if (!baseplayah) return original_clientinput(baseplayah, ModelState);
			if (!baseplayah->IsValid()) return original_clientinput(baseplayah, ModelState);

			auto* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
			Vector3 startPos = LocalPlayer::Entity()->get_bone_pos(head);
			Vector3 endPos = ((BasePlayer*)TargetPlayer)->get_bone_pos(head);

			
				static auto Facz = AssetBundle::LoadFromFile("C:\\Desector.desc");
				static auto Facz2 = AssetBundle::LoadFromFile("C:\\invis_bundles");

				if (vars::visuals::hand_chams_type == 0)
				{
					Gold = Facz->LoadAsset<Material>("el_designeffect.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 1)
				{
					Glitter = Facz2->LoadAsset<Material>("chams2.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 2)
				{
					gaga = Facz2->LoadAsset<Material>("chams3.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 3)
				{
					Pixel = Facz2->LoadAsset<Material>("chams4.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 4)
				{
					gagahga = Facz2->LoadAsset<Material>("chams5.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 5)
				{
					Ghost = Facz2->LoadAsset<Material>("chams6.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 6)
				{
					GredSpace = Facz2->LoadAsset<Material>("chams7.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 7)
				{
					Glitch2 = Facz2->LoadAsset<Material>("chams8.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 8)
				{
					Glitch3 = Facz2->LoadAsset<Material>("chams9.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 9)
				{
					Slick = Facz2->LoadAsset<Material>("chams10.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 10)
				{
					afagfgtqag = Facz2->LoadAsset<Material>("chams11.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 11)
				{
					Ghost2 = Facz2->LoadAsset<Material>("chams12.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 12)
				{
					GredSpace = Facz2->LoadAsset<Material>("chams13.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 13)
				{
					GalaxyFire = Facz2->LoadAsset<Material>("chams14.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));
				}

				if (vars::visuals::hand_chams_type == 14)
				{
					GalaxyNightFire = Facz2->LoadAsset<Material>("chams15.mat", Type::GetType(("UnityEngine.Material, UnityEngine.CoreModule")));	
				}


			if (vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key))
				baseplayah->clientTickInterval() = 0.99f;
			else {
				baseplayah->clientTickInterval() = 0.05f;
			}
			/*if (vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key)) {
				other::find_manipulate_angle();
			
			}
			else if (!other::m_manipulate.empty()) {
				other::m_manipulate = Vector3::Zero();
			}*/
			
			typedef void(__stdcall* oxunem)(float);
			if (vars::misc::speedhack && GetAsyncKeyState(vars::keys::speedkey)) {
				oxunem Infinity = (oxunem)(vars::stor::gBase + CO::set_timeScale);
				Infinity(2);
			}
			else {
				oxunem Infinity = (oxunem)(vars::stor::gBase + CO::set_timeScale);
				Infinity(1);
			}

			if (vars::misc::mass_suicide) {
				LocalPlayer::Entity()->OnLand(-108.0001f);
			}
		
			if (vars::misc::movement_line) {
				if (baseplayah->get_flag(PlayerFlags::Connected)) {
					DDraw::Line(baseplayah->_playerModel()->position(), baseplayah->lastSentTick()->tick_position(), Color(vars::colors::mov_line.x, vars::colors::mov_line.y, vars::colors::mov_line.z, 255), 1.f, true, true);
				}
				else {}
			}

			if (vars::misc::flyhack_indicator) {
				CheckFlyhack();
			}

			if (vars::combat::bullet_tp && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr) {
				auto transform = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->transform_(head);
				if (transform) {
					transform->set_rotation(Quaternion(0.f, 0.f, 0.f, 1.f));
				}
			}
			
			

			auto held = baseplayah->GetHeldEntity<BaseProjectile>();
			auto ishak = baseplayah->GetHeldEntity<BaseMelee>();
			Vector3 target_pos = target_player->transform()->position();
			float distance = Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), target_pos);
			BaseProjectile* _held = held = baseplayah->GetHeldEntity<BaseProjectile>();
		

			float lastShotTime = _held->lastShotTime() - GLOBAL_TIME;
			float reloadTime = _held->nextReloadTime() - GLOBAL_TIME;

			float desyncpercentage;
			float desyncTime = (Time::realtimeSinceStartup() - baseplayah->lastSentTickTime()) - 0.03125 * 3;
			desyncpercentage = ((desyncTime / 0.99f) * 100.0f) + 1.f;
			variables::manipulator::desync = (0.1f + (((desyncTime)+2.f / 60.f + 0.125f) * 1.5f) * baseplayah->GetMaxSpeed()) - 0.05f;

			if (_held && _held != nullptr && _held->class_name_hash() == STATIC_CRC32("BaseProjectile") ||
				_held && _held != nullptr && _held->class_name_hash() == STATIC_CRC32("BowWeapon") ||
				_held && _held != nullptr && _held->class_name_hash() == STATIC_CRC32("CrossBowWeapon")) {
				if (vars::combat::autoreload && _held)
				{
					BaseProjectile* ent = baseplayah->GetHeldEntity<BaseProjectile>();

					if (!did_reload)
						time_since_last_shot = (Time::fixedTime() - fixed_time_last_shot);

					if (just_shot && (time_since_last_shot > 0.2f))
					{
						ent->_ServerRPC(("StartReload"));
						ent->SendSignalBroadcast(BaseEntity::Signal::Reload);
						just_shot = false;
					}
					if (time_since_last_shot > (_held->reloadTime() - (_held->reloadTime() / 10))
						&& !did_reload)
					{
						ent->_ServerRPC(("Reload"));
						did_reload = true;
						time_since_last_shot = 0;
					}
				}
			}
			else {
				did_reload = false;
				just_shot = true;
				fixed_time_last_shot = Time::fixedTime();
			}		

			if (vars::combat::bullet_tp && vars::combat::hitscan && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr) {
				Vector3 z = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);
				Vector3 eye = LocalPlayer::Entity()->eyes()->position();

				float radius = 2.5f;

				if (!utils::LineOfSight(z, eye)) {
					if (utils::LineOfSight(eye, z + Vector3(0, radius, 0)))
						vars::stuff::best_target = z + Vector3(0, radius, 0);
					if (!utils::LineOfSight(eye, z + Vector3(0, radius, 0)) && utils::LineOfSight(eye, z + Vector3(radius, 0, 0)))
						vars::stuff::best_target = z + Vector3(radius, 0, 0);
					if (!utils::LineOfSight(eye, z + Vector3(radius, 0, 0)) && utils::LineOfSight(eye, z + Vector3(-radius, 0, 0)))
						vars::stuff::best_target = z + Vector3(-radius, 0, 0);
					if (!utils::LineOfSight(eye, z + Vector3(-radius, 0, 0)) && utils::LineOfSight(eye, z + Vector3(0, 0, radius)))
						vars::stuff::best_target = z + Vector3(0, 0, radius);
					if (!utils::LineOfSight(eye, z + Vector3(0, 0, radius)) && utils::LineOfSight(eye, z + Vector3(0, 0, -radius)))
						vars::stuff::best_target = z + Vector3(0, 0, -radius);

					DDraw::Sphere(vars::stuff::best_target, 0.2f, Color::Color(0, 1, 0, 50), 0.01f, false);
				}
				else
					vars::stuff::best_target = Vector3(0, 0, 0);
			}
			else
				vars::stuff::best_target = Vector3(0, 0, 0);

			Vector3 target;
			if (vars::combat::hitscan) {
				if (vars::stuff::best_target != Vector3(0, 0, 0))
				{
					target = vars::stuff::best_target;
				}
				else
				{
					target = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);
				}
			}
			else
				target = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);
			bool autoshoot_cheack1 = GetAsyncKeyState(vars::keys::lv_key);
			if (baseplayah->userID() == LocalPlayer::Entity()->userID()) {
				if (LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1588298435 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -778367295 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1367281941 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -765183617 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -41440462 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 795371088 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -75944661 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1965232394 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 884424049 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1443579727 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1318558775 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1796682209 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 649912614 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 818877484 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -852563019 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1373971859 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1758372725 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1812555177 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -904863145 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1545779598 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1335497659 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -2069578888 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1214542497 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 28201841 ||
					LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1953903201) {
					if (vars::combat::desync_autoshoot_type == 0) {
						if (vars::combat::autoshoot) {
							if (vars::combat::autoshoot_type == 0) {
								if (!held->Empty()) {
									if (_held && reloadTime < 1.f) {
										if (lastShotTime < -0.1f) {
											if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && !LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->userID()) && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->IsValid() && baseplayah->get_flag(PlayerFlags::Connected)/* && !did_reload*/) {
												if (utils::LineOfSight(target, baseplayah->eyes()->position())) {
													Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
													DWORD64 active = weapon->entity();
													DWORD64 Held = weapon->entity();
													bool deployed = read(Held + O::HeldEntity::isDeployed, bool);
													float curtime = LocalPlayer::Entity()->lastSentTickTime();
												
													float last_shoot_time = _held->lastShotTime() - GLOBAL_TIME;
													float reloadTime = _held->nextReloadTime() - GLOBAL_TIME;
													if (_held->class_name_hash() == STATIC_CRC32("BowWeapon"))
													{
														if (!held->HasReloadCooldown() && !held->HasAttackCooldown() && reloadTime < 1.0f &&
															!held->Empty() && target_player != nullptr && target_player->IsValid() && LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected) && held->nextAttackTime() < Time::time())
														{
															lastshottime = Time::fixedTime();
															held->SendSignalBroadcast(BaseEntity::Signal::Attack,(""));
															held->DoAttack();
															held->LaunchProjectile();
															held->primaryMagazine()->contents()--;
															held->UpdateAmmoDisplay();
															held->ShotFired();	
															held->DidAttackClientside();
															held->BeginCycle();
															held->StartAttackCooldown(held->repeatDelay());
														}
													}
													else {
														lastshottime = Time::fixedTime();
														held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
														held->DoAttack();
														held->LaunchProjectile();
														held->primaryMagazine()->contents()--;
														held->UpdateAmmoDisplay();
														held->ShotFired();
														held->DidAttackClientside();
														held->BeginCycle();
														held->StartAttackCooldown(held->repeatDelay());
													}
												}
											}
										}
									}
								}
							}
						}
					
						if (vars::combat::autoshoot) {
							if (vars::combat::autoshoot_type == 1) {
								if (!held->Empty() && held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
									if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->isCached()) {
										auto mpv = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->find_mpv_bone();
										Vector3 target;
										if (mpv != nullptr)
											target = mpv->position;
										else
											target = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);

										if (utils::LineOfSight(target, baseplayah->eyes()->position()))
											held->DoAttack();
									}
								}
							}
						}
					}

					if (vars::combat::desync_autoshoot_type == 1) {
						if (vars::combat::autoshoot && vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key)) {
							if (vars::combat::autoshoot_type == 0) {
								if (!held->Empty()) {
									if (_held && reloadTime < 1.f) {
										if (lastShotTime < -0.1f) {
											if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && !LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->userID()) && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->IsValid() && baseplayah->get_flag(PlayerFlags::Connected)/* && !did_reload*/) {
												if (utils::LineOfSight(target, baseplayah->eyes()->position())) {
													Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
													DWORD64 active = weapon->entity();
													DWORD64 Held = weapon->entity();
													bool deployed = read(Held + O::HeldEntity::isDeployed, bool);
													float curtime = LocalPlayer::Entity()->lastSentTickTime();
													float last_shoot_time = _held->lastShotTime() - GLOBAL_TIME;
													float reloadTime = _held->nextReloadTime() - GLOBAL_TIME;
													if (_held->class_name_hash() == STATIC_CRC32("BowWeapon"))
													{
														if (!held->HasReloadCooldown() && !held->HasAttackCooldown() && reloadTime < 1.0f &&
															!held->Empty() && target_player != nullptr && target_player->IsValid() && LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected) && held->nextAttackTime() < Time::time())
														{
															lastshottime = Time::fixedTime();
															held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
															held->DoAttack();
															held->LaunchProjectile();
															held->primaryMagazine()->contents()--;
															held->UpdateAmmoDisplay();
															held->ShotFired();
															held->DidAttackClientside();
															held->BeginCycle();
															held->StartAttackCooldown(held->repeatDelay());
														}
													}
													else {
														lastshottime = Time::fixedTime();
														held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
														held->DoAttack();
														held->LaunchProjectile();
														held->primaryMagazine()->contents()--;
														held->UpdateAmmoDisplay();
														held->ShotFired();
														held->DidAttackClientside();
														held->BeginCycle();
														held->StartAttackCooldown(held->repeatDelay());
													}
												}
											}
										}
									}
								}
							}
						}
				
						if (vars::combat::autoshoot && vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key)) {
							if (vars::combat::autoshoot_type == 1) {
								if (!held->Empty() && held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
									if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->isCached()) {
										auto mpv = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->find_mpv_bone();
										Vector3 target;
										if (mpv != nullptr)
											target = mpv->position;
										else
											target = target;

										if (utils::LineOfSight(target, baseplayah->eyes()->position()))
											LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
											held->DoAttack();
									}
								}
							}
						}
					}

					if (vars::combat::instakill && GetAsyncKeyState(vars::keys::instakill)) {
						if (vars::combat::instakill_type == 0) {
							if (!held->Empty()) {
								if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && !LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->userID()) && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->IsValid() && baseplayah->get_flag(PlayerFlags::Connected)/* && !did_reload*/) {
									if (utils::LineOfSight(target, baseplayah->eyes()->position())) {
										baseplayah->clientTickInterval() = 0.99f;
										if (desyncTime > 0.80f) {
											lastshottime = Time::fixedTime();
											held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
											held->DoAttack();
											held->LaunchProjectile();
											held->primaryMagazine()->contents()--;
											held->UpdateAmmoDisplay();
											held->ShotFired();
											held->DidAttackClientside();
											held->BeginCycle();
											held->StartAttackCooldown(held->repeatDelay());
										}
									}
								}
							}
						}
					}
					if (autoshoot_cheack1) {
						if (held && _held->class_name_hash() == STATIC_CRC32("BaseProjectile"))
						{
							if (_held->HasAttackCooldown() || _held->nextAttackTime() >= Time::time() || _held->timeSinceDeploy() < _held->deployDelay()) {
							}
							else
							{
								if (!held->Empty()) {
									if (reloadTime < 1.0f && lastShotTime < -0.1f) {
										if (reinterpret_cast<BasePlayer*>(target_player) != nullptr && !LocalPlayer::Entity()->is_teammate((uintptr_t)reinterpret_cast<BasePlayer*>(target_player))) {
											lastshottime = Time::fixedTime();
											held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
											held->DoAttack();
											held->LaunchProjectile();
											held->primaryMagazine()->contents()--;
											held->UpdateAmmoDisplay();
											held->ShotFired();
											held->DidAttackClientside();
											held->BeginCycle();
											held->StartAttackCooldown(held->repeatDelay());
										}
									}
								}
							}

						}
						else
						{
							if (reloadTime < 1.0f && lastShotTime < -0.1f) {
								if (reinterpret_cast<BasePlayer*>(target_player) != nullptr && !LocalPlayer::Entity()->is_teammate((uintptr_t)reinterpret_cast<BasePlayer*>(target_player))) {
									lastshottime = Time::fixedTime();
									held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
									held->DoAttack();
									held->LaunchProjectile();
									held->primaryMagazine()->contents()--;
									held->UpdateAmmoDisplay();
									held->ShotFired();
									held->DidAttackClientside();
									held->BeginCycle();
									held->StartAttackCooldown(held->repeatDelay());
								}
							}
						}
					}
					}
					else {
						did_reload = false;
						just_shot = true;
						fixed_time_last_shot = Time::fixedTime();
					}

				static bool alreadystartedReload = false;

				static bool alreadyReset = false;

				if (vars::weapons::compound && held->class_name_hash() == STATIC_CRC32("CompoundBowWeapon")) {
					reinterpret_cast<CompoundBowWeapon*>(held)->currentHoldProgress() = 1.5f;
				}

				GLOBAL_TIME = Time::time();
			}

			Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
			DWORD64 active = weapon->entity();
			char* classname = weapon->ClassName();
			bool weaponmelee = weapon && classname && (strcmp(classname, ("BaseMelee")) || strcmp(classname, ("Jackhammer")));
			if (active && vars::misc::weapon_spam) {
				if (GetAsyncKeyState(vars::keys::weaponspam)) {
					reinterpret_cast<void(*)(uintptr_t, Signal, Str)>(vars::stor::gBase + CO::SendSignalBroadcast)(active, Signal::Attack, Str((L"")));
				}
			}
			if (vars::stor::meme_target != NULL) {
				if (!reinterpret_cast<BasePlayer*>(vars::stor::meme_target)->IsValid()) {
					vars::stor::meme_target = NULL;
				}
				if (LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::meme_target)->userID())) {
					vars::stor::meme_target = NULL;
				}
				if (reinterpret_cast<BasePlayer*>(vars::stor::meme_target)->health() <= 0.f) {
					vars::stor::meme_target = NULL;
				}
			}

			lol::auto_farm_loop(weaponmelee, active);
			//game_thread_loop();

			DWORD64 BaseNetworkable;
			BaseNetworkable = read(vars::stor::gBase + CO::BaseNetworkable, DWORD64); //BaseNetworkable_c 
			DWORD64 EntityRealm = read(BaseNetworkable + 0xB8, DWORD64);
			DWORD64 ClientEntities = read(EntityRealm, DWORD64);
			DWORD64 ClientEntities_list = read(ClientEntities + 0x10, DWORD64);
			DWORD64 ClientEntities_values = read(ClientEntities_list + 0x28, DWORD64);
			DWORD64 EntityBuffer = read(ClientEntities_values + 0x18, DWORD64);
			int EntityCount = read(ClientEntities_values + 0x10, int);

			for (int i = 0; i <= EntityCount; i++)
			{
				DWORD64 Entity = read(EntityBuffer + 0x20 + (i * 0x8), DWORD64);
				if (Entity <= 100000) continue;
				DWORD64 Object = read(Entity + 0x10, DWORD64);
				if (Object <= 100000) continue;
				DWORD64 ObjectClass = read(Object + 0x30, DWORD64);
				if (ObjectClass <= 100000) continue;
				pUncStr name = read(ObjectClass + 0x60, pUncStr);
				DWORD64 ent = read(Object + 0x28, UINT64);
				if (!name) continue;
				char* buff = name->stub;
				BasePlayer* Player = (BasePlayer*)read(Object + 0x28, DWORD64);
				if (strstr(buff, ("Local"))) {
					BasePlayer* Player = (BasePlayer*)read(Object + 0x28, DWORD64);
					BasePlayer* Local = (BasePlayer*)read(Object + 0x28, DWORD64);
					if (!read(Player + O::BasePlayer::playerModel, DWORD64)) continue;

					HandChams();
				}

				if (!reinterpret_cast<BaseEntity*>(Entity)->IsValid()) { continue; }
				if (!reinterpret_cast<Component*>(Entity)->gameObject()) { continue; }
				if (strstr(buff, ("Local"))) { continue; }

				Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
				DWORD64 active = weapon->entity();
				char* classname = weapon->ClassName();

				bool weaponmelee = weapon && classname && (strcmp(classname, ("BaseMelee")) || strcmp(classname, ("Jackhammer")));
				if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer()) {
					BasePlayer* lol = (BasePlayer*)Entity;

					if (!lol) continue;

					if (held->class_name_hash() == STATIC_CRC32("BaseMelee") || held->class_name_hash() == STATIC_CRC32("Jackhammer")) {
						if (vars::combat::silent_melee && weaponmelee && Math::Distance_3D(lol->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= vars::combat::reach) {
							f_object target = f_object::get_melee_target((BasePlayer*)Entity, active);
							lol::do_attack(target, active, true);
						}
					}
				}

				if (held->class_name_hash() == STATIC_CRC32("Hammer")) {
					if (vars::combat::hammerspam && weaponmelee && Math::Distance_3D(baseplayah->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= vars::combat::reach) {
						f_object target = f_object::get_spam1_target((BasePlayer*)Entity, active);
						lol::hammerspam_attack(target, active, true);
					}
				}

				if (held->class_name_hash() == STATIC_CRC32("Hammer")) {
					if (vars::combat::hammerspam1 && weaponmelee && Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= vars::combat::reach) {
						f_object target = f_object::get_spam_target((BasePlayer*)Entity, active);
						lol::hammerspam1_attack(target, active, true);
					}
				}

				if (vars::misc::auto_pickup && strstr(buff, ("/collectable/"))) {
					UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
					Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
					if (Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f) {
						utils::ServerRPC(Entity, Str((L"Pickup")));
					}
				}
			}

			if (vars::visuals::hand_chams) {
				auto oxunem = il2cpp::_init_class(("Graphics"), ("ConVar"));
				auto static_fields = safe_read(oxunem + 0xb8, uintptr_t);
				safe_write(static_fields + 0x75, false, bool); // public static bool viewModelCamera;
			}

			Physics::IgnoreLayerCollision((int)Layer::PlayerMovement, (int)Layer::Water, !vars::misc::walker);
			Physics::IgnoreLayerCollision((int)Layer::PlayerMovement, (int)Layer::Tree, vars::misc::walker);
			Physics::IgnoreLayerCollision((int)Layer::PlayerMovement, (int)Layer::AI, vars::misc::walker);

			weapon_set();
			misc_set();
			
			//auto entityList = BaseNetworkable::clientEntities()->entityList();
			//if (entityList) {
			//	for (int i = 0; i < entityList->vals->size; i++) {
			//		uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			//		if (!Entity) continue;
			//		uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
			//		if (!Object) continue;
			//		uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
			//		if (!ObjectClass) continue;

			//		pUncStr name = read(ObjectClass + 0x60, pUncStr); if (!name) continue;
			//		char* buff = name->stub;

			//		if (!reinterpret_cast<BaseEntity*>(Entity)->IsValid()) { continue; }
			//		if (!reinterpret_cast<Component*>(Entity)->gameObject()) { continue; }
			//		if (strstr(buff, ("Local"))) { continue; }

			//		Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
			//		DWORD64 active = weapon->entity();
			//		char* classname = weapon->ClassName();

			//		bool weaponmelee = weapon && classname && (strcmp(classname, ("BaseMelee")) || strcmp(classname, ("Jackhammer")));
			//		if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer()) {
			//			BasePlayer* lol = (BasePlayer*)Entity;

			//			if (!lol) continue;

			//			BaseMelee* melee = LocalPlayer::Entity()->GetHeldEntity<BaseMelee>();

			//			if (melee->_class_name_hash() == STATIC_CRC32("BaseMelee") && melee->_class_name_hash() == STATIC_CRC32("Jackhammer")) {
			//				if (vars::combat::silent_melee && held->class_name_hash() == STATIC_CRC32("") && Math::Distance_3D(baseplayah->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= vars::combat::reach) {
			//					f_object target = f_object::get_melee_target((BasePlayer*)Entity, active);
			//					lol::do_attack(target, active, true);
			//				}
			//			}
			//			else {

			//			}

			//			if (LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 200773292) {
			//				if (vars::combat::hammerspam && weaponmelee && Math::Distance_3D(baseplayah->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= vars::combat::reach) {
			//					f_object target = f_object::get_spam_target((BasePlayer*)Entity, active);
			//					lol::hammerspam_attack(target, active, true);
			//				}
			//			}
			//			else {

			//			}

			//			if (vars::misc::auto_revive) {
			//				UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
			//				Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
			//				if (vars::misc::auto_revive && (BasePlayer*)Entity && lol->get_flag(PlayerFlags::Wounded) && Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f && LocalPlayer::Entity()->GetKeyState(Button::USE)) {
			//					lol::pickup_player((BasePlayer*)Entity);
			//				}
			//			}
			//		}
			//		if (vars::misc::auto_pickup && strstr(buff, ("/collectable/"))) {
			//			UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
			//			Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
			//			if (Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f) {
			//				utils::ServerRPC(Entity, Str((L"Pickup")));
			//			}
			//		}
			//	}
			//}

			original_clientinput(baseplayah, ModelState);
			if (vars::misc::AspectRatio)
				Camera::SetAspect(vars::misc::ratio);

			if (vars::misc::model_mem) {
				LocalPlayer::Entity()->playerModel()->PlayCinematic(il2cpp::String::New(("Debug_TPose")));
			}
			if (vars::combat::teleport && GetAsyncKeyState(vars::keys::teleport)) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Mounted);
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::OnGround);
			}
			if (vars::misc::modelstate == 0) {
			}
			if (vars::misc::modelstate == 1) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::OnLadder);
			}
			if (vars::misc::modelstate == 2) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::OnGround);
			}
			if (vars::misc::modelstate == 3) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Ducked);
			}
			if (vars::misc::modelstate == 4) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Sleeping);
			}

			if (vars::misc::omnidirectional_sprinting) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Sprinting);
			}

			if (vars::misc::interactive_debug && GetAsyncKeyState(vars::keys::debugging)) {
				BaseProjectile* ent = baseplayah->GetHeldEntity<BaseProjectile>();
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Mounted);
				ent->_ServerRPC(("RPC_LootPlayer"));
			}
		}

		void UpdateAmbient(TOD_Sky* TOD_Sky) {
			uintptr_t cycle = read(TOD_Sky + 0x38, uintptr_t);
			uintptr_t ambient = read(TOD_Sky + 0x90, uintptr_t);
			if (vars::misc::night_stars) {
				uintptr_t stars = read(TOD_Sky + 0x70, uintptr_t);
				*(float*)(stars + 0x14) = vars::misc::stars;
			}

			if (!vars::misc::bright_ambient) {
				return original_updateambient(TOD_Sky);
			}
			RenderSettings::set_ambientMode(RenderSettings::AmbientMode::Flat);
			RenderSettings::set_ambientIntensity(6.f);
			RenderSettings::set_ambientLight(Color({ vars::colors::ambient_color.x, vars::colors::ambient_color.y, vars::colors::ambient_color.z, 1 }));
		}
		pUncStr Run(ConsoleOptions* options, pUncStr strCommand, DWORD64 args) {
			if (options->IsFromServer()) {
				std::wstring cmd = std::wstring(strCommand->str);
				if (cmd.find((L"noclip")) != std::wstring::npos || cmd.find((L"debugcamera")) != std::wstring::npos || cmd.find((L"camspeed")) != std::wstring::npos || cmd.find((L"admintime")) != std::wstring::npos) {
					strCommand = nullptr;
				}
			}
			return original_consolerun(options, strCommand, args);
		}


		void DoHitNotify(BaseCombatEntity* entity, HitInfo* info, Projectile* prj) {
			if (entity->IsPlayer()) {
				if (vars::misc::hit_logs) {
				

					//NotificationSystem.AddNotification("HI!", 2500);
				}

				if (vars::misc::hitmaterial == 0) {
				}
				if (vars::misc::hitmaterial == 1) {
					uint32_t material = utils::StringPool::Get(("glass"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 2) {
					uint32_t material = utils::StringPool::Get(("water"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 3) {
					uint32_t material = utils::StringPool::Get(("wood"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 4) {
					uint32_t material = utils::StringPool::Get(("metal"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 5) {
					uint32_t material = utils::StringPool::Get(("sand"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 6) {
					uint32_t material = utils::StringPool::Get(("grass"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 7) {
					uint32_t material = utils::StringPool::Get(("rock"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 8) {
					uint32_t material = utils::StringPool::Get(("concrete"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 9) {
					uint32_t material = utils::StringPool::Get(("forest"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 10) {
					uint32_t material = utils::StringPool::Get(("cloth"));
					info->HitMaterial() = material;
				}
				if (vars::misc::hitmaterial == 11) {
					uint32_t material = utils::StringPool::Get(("null"));
					info->HitMaterial() = material;
				}
				if (vars::misc::custom_hitsound) {

					if (vars::misc::hitsound_rejim == 0) {
						PlaySoundA((LPCSTR)NL, NULL, SND_MEMORY | SND_ASYNC);
					}
					if (vars::misc::hitsound_rejim == 0) {
						PlaySoundA((LPCSTR)rb, NULL, SND_MEMORY | SND_ASYNC);
					}

					return;
				}
				if (vars::combat::always_headshot) {
					reinterpret_cast<void(*)(Str, GameObject*)>(vars::stor::gBase + CO::EffectRun)(
						Str((L"assets/bundled/prefabs/fx/headshot_2d.prefab")),
						LocalPlayer::Entity()->eyes()->gameObject());
					return;
				}
			}

			return original_dohitnotify(entity, info, prj);
		}
		bool get_isHeadshot(HitInfo* hitinfo) {
			if (vars::misc::custom_hitsound) {
				return false;
			}
			if (vars::combat::always_headshot) {
				return false;
			}
			return original_getisheadshot(hitinfo);
		}
		void Play(ViewModel* viewmodel, pUncStr name, int layer = 0) {
			if (vars::weapons::remove_attack_anim) {
				static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::DoAttack(): Void");
				if (!CALLED_BY(ptr, 0x296) || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -75944661) {
					return original_viewmodelplay(viewmodel, name, layer);
				}
			}
			else {
				return original_viewmodelplay(viewmodel, name, layer);
			}
		}
	}
	namespace combat {
		float GetRandomVelocity(ItemModProjectile* mod) {
			return vars::weapons::fast_bullets ? original_getrandomvelocity(mod) * 1.499 : original_getrandomvelocity(mod);
		}
		void AddPunch(HeldEntity* a1, Vector3 a2, float duration) {
			a2 *= vars::weapons::recoil_control / 100.f;
			return original_addpunch(a1, a2, duration);
		}
		Vector3 MoveTowards(Vector3 a1, Vector3 a2, float maxDelta) {
			static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::SimulateAimcone(): Void");
			if (CALLED_BY(ptr, 0x800)) {
				a2 *= vars::weapons::recoil_control / 100.f;
				maxDelta *= vars::weapons::recoil_control / 100.f;
			}
			return original_movetowards(a1, a2, maxDelta);
		}
		bool DoHit(Projectile* prj, HitTest* test, Vector3 point, Vector3 normal) {
			auto localPlayer = LocalPlayer::Entity();
			auto held = localPlayer->GetHeldEntity<BaseProjectile>();

			static float r = 1.00f, g = 0.00f, b = 1.00f;
			static int cases = 0;
			switch (cases) {
			case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
			case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
			case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
			case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
			default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
			}

			if (prj->isAuthoritative()) {
				if (vars::combat::ignore_team) {
					if (test->HitEntity() != null) {
						if (test->HitEntity()->IsValid()) {
							if (LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(test->HitEntity())->userID())) {
								if (reinterpret_cast<BaseCombatEntity*>(test->HitEntity())->IsPlayer()) {
									return false;
								}
							}
						}
					}
				}

				if (vars::misc::hitmarker) {
					if (test->HitEntity() != nullptr) {
						if (test->HitEntity()->IsValid()) {
							if (reinterpret_cast<BasePlayer*>(test->HitEntity())->IsPlayer()) {
								DDraw::Text(il2cpp::String::New("x"), prj->currentPosition(), Color(vars::colors::marker.x, vars::colors::marker.y, vars::colors::marker.z, 255), 1.5f);
							}
						}
					}
				}

				if (vars::weapons::penetrate) {
					if (test->HitEntity() != null) {
						if (test->HitEntity()->IsValid()) {
							BaseCombatEntity* lol = reinterpret_cast<BaseCombatEntity*>(test->HitEntity());
							if (vars::stuff::testInt == 2) {
								printf(("%s \n"), lol->ClassName());
							}
							if (lol->ClassNameHash() == STATIC_CRC32("CargoShip") || lol->ClassNameHash() == STATIC_CRC32("BaseOven")
								|| lol->ClassNameHash() == STATIC_CRC32("TreeEntity") || lol->ClassNameHash() == STATIC_CRC32("OreResourceEntity")
								|| lol->ClassNameHash() == STATIC_CRC32("CH47HelicopterAIController") || lol->ClassNameHash() == STATIC_CRC32("MiniCopter")
								|| lol->ClassNameHash() == STATIC_CRC32("BoxStorage") || lol->ClassNameHash() == STATIC_CRC32("Workbench")
								|| lol->ClassNameHash() == STATIC_CRC32("VendingMachine") || lol->ClassNameHash() == STATIC_CRC32("Barricade")
								|| lol->ClassNameHash() == STATIC_CRC32("BuildingPrivlidge") || lol->ClassNameHash() == STATIC_CRC32("LootContainer")
								|| lol->ClassNameHash() == STATIC_CRC32("HackableLockedCrate") || lol->ClassNameHash() == STATIC_CRC32("ResourceEntity")
								|| lol->ClassNameHash() == STATIC_CRC32("RidableHorse") || lol->ClassNameHash() == STATIC_CRC32("MotorRowboat")
								|| lol->ClassNameHash() == STATIC_CRC32("ScrapTransportHelicopter") || lol->ClassNameHash() == STATIC_CRC32("JunkPile")
								|| lol->ClassNameHash() == STATIC_CRC32("MiningQuarry") || lol->ClassNameHash() == STATIC_CRC32("WaterCatcher")) {
								return false;
							}
						}
					}
				}
			}
			return original_dohit(prj, test, point, normal);
		}
		void Launch(Projectile* prdoj) {
			if (vars::weapons::no_spread) {
				write(prdoj->mod() + 0x38, 0.f, float);
			}
			return original_launch(prdoj);
		}
		bool CanHoldItems(BaseMountable* a1) {
			if (vars::weapons::minicopter_aim) return true;
			return original_canholditems(a1);
		}
		void SendProjectileAttack(BasePlayer* a1, PlayerProjectileAttack* a2) {
			uintptr_t PlayerAttack = read(a2 + 0x18, uintptr_t); // PlayerAttack playerAttack;
			uintptr_t Attack = read(PlayerAttack + 0x18, uintptr_t); // public Attack attack;
			uint32_t hitID = read(Attack + 0x2C, uint32_t);
			if (vars::weapons::spoof_hitdistance) {
				write(a2 + 0x2C, vars::weapons::hitdistance, float);
			}
			if (vars::combat::always_headshot || vars::combat::always_heli_rotor) {
				BaseCombatEntity* entity = BaseNetworkable::clientEntities()->Find<BaseCombatEntity*>(hitID);
				if (vars::combat::always_headshot) {
					if (entity->IsPlayer()) {
						uint32_t bone = utils::StringPool::Get(("head"));
						write(Attack + 0x30, bone, uint32_t);
					}
				}
				if (vars::combat::always_heli_rotor) {
					if (entity->ClassNameHash() == STATIC_CRC32("BaseHelicopter")) {
						int health = (int)reinterpret_cast<BaseCombatEntity*>(vars::stor::closestHeli)->health();
						if (health <= 5000) {
							write(Attack + 0x30, utils::StringPool::Get(("tail_rotor_col")), uint32_t);
						}
						else {
							write(Attack + 0x30, utils::StringPool::Get(("engine_col")), uint32_t);
						}
					}
				}
			}
			return original_sendprojectileattack(a1, a2);
		}
		bool CanAttack(BasePlayer* a1) {
			if (vars::misc::can_attack)
				return true;
			return original_canattack(a1);
		}
		Projectile* CreateProjectile(BaseProjectile* BaseProjectileA, void* prefab_pathptr, Vector3 pos, Vector3 forward, Vector3 velocity) {
			Projectile* projectile = original_create_projectile(BaseProjectileA, prefab_pathptr, pos, forward, velocity);
			BasePlayer* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
			//projectile->transform()->set_scale({ 8, 8 ,8 });

			if (vars::weapons::thick_bullet) {
				projectile->thickness() = vars::weapons::thickness;
			}
			else {
				projectile->thickness() = 0.1f;
			}

			float desyncTime = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime()) - 0.03125 * 3;

			return projectile;
		}
		Vector3 GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
			BasePlayer* TargetPlayer = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
			Vector3 Local = LocalPlayer::Entity()->eyes()->get_position();
			Vector3 heli_target = reinterpret_cast<BaseEntity*>(vars::stor::closestHeli)->transform()->position() + Vector3(0, 1.5, 0);

			Vector3 target;

			if (!reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)) {

			}
			else {
				if (vars::combat::hitscan) {
					if (vars::stuff::best_target != Vector3(0, 0, 0))
					{
						target = vars::stuff::best_target;
					}
					else
					{
						target = vars::combat::onal == 0 ? reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head) : reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(spine1);
					}
				}
				else
					target = vars::combat::onal == 0 ? reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head) : reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(spine1);
			}

				if (vars::combat::psilent) {
					auto* target_player = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);
					auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
					BaseProjectile* _held = held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
					auto mag = held->primaryMagazine();
					if (!mag) return original_aimconedirection(aimCone, inputVec, anywhereInside);
					auto ammo = mag->ammoType();
					if (!ammo) return original_aimconedirection(aimCone, inputVec, anywhereInside);
					auto mod = ammo->GetComponent<ItemModProjectile>(Type::ItemModProjectile());
					if (!mod) return original_aimconedirection(aimCone, inputVec, anywhereInside);
					auto projectile = mod->projectileObject()->Get()->GetComponent<Projectile>(Type::Projectile());
					if (!projectile) return original_aimconedirection(aimCone, inputVec, anywhereInside);

					if (projectile)
					{
						auto projectileVelocity = mod->projectileVelocity();
						auto projectileVelocityScale = _held->projectileVelocityScale();

						if (vars::weapons::fast_bullets) {
							projectileVelocityScale = (1.500f)*_held->projectileVelocityScale();
						}
						else
						{
							projectileVelocityScale = (1.000f)*_held->projectileVelocityScale();
						}

						float drag = projectile->drag();
						float gravityModifier = projectile->gravityModifier();
						float initialDistance = projectile->initialDistance();
						auto gravity = Physics::get_gravity();
						auto deltaTime = Time::fixedDeltaTime();
						auto timescale = Time::timeScale();

						float DeltaForLoop = deltaTime * timescale;

						Vector3 actualTargetPos = target;

						Vector3 localPos = LocalPlayer::Entity()->eyes()->get_position();

						float bulletTime = std::sqrt((actualTargetPos.x) * (actualTargetPos.x) + (actualTargetPos.z) * (actualTargetPos.z));

						Vector3 targetvel = target_player->GetWorldVelocity();
						Vector3 targetPosition = actualTargetPos;

						auto _aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);

						auto position = localPos;
						float num = 0.03125f;
						int num3 = (8.f / DeltaForLoop);

						int simulations = 0;

						while (simulations < 30)
						{
							simulations++;
							bool hitPlayer = false;

							_aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);
							Vector3 velocity = _aimDir * projectileVelocity * projectileVelocityScale;

							auto currentPosition = localPos;
							auto previousPosition = currentPosition;


							Vector3 closestPoint(FLT_MAX, FLT_MAX, FLT_MAX);
							Vector3 offset = Vector3().Zero();

							for (int i = -1; i < num3; i++)
							{
								previousPosition = currentPosition;
								currentPosition += velocity * deltaTime;
								velocity += gravity * gravityModifier * deltaTime;
								velocity -= velocity * drag * deltaTime;

								auto line = (currentPosition - previousPosition);
								auto len = line.UnityMagnitude();
								line.UnityNormalize();
								auto v = actualTargetPos - previousPosition;
								auto d = Vector3().UnityDot(v, line);

								if (d < 0.f)
								{
									d = 0.f;
								}
								else if (d > len)
								{
									d = len;
								}

								Vector3 nearestPoint = previousPosition + line * d;

								if (nearestPoint.distance(actualTargetPos) < 0.1f)
								{
									bulletTime = i * num;
									hitPlayer = true;
								}
								else if (nearestPoint.distance(actualTargetPos) < closestPoint.distance(actualTargetPos))
								{
									closestPoint = nearestPoint;
									offset = actualTargetPos - nearestPoint;
								}
							}
							if (hitPlayer) break;
							targetPosition += offset;
						}


						if (bulletTime != 1337.f)
						{
							Vector3 finalVelocity = Vector3(TargetPlayer->GetWorldVelocity().x, 0, TargetPlayer->GetWorldVelocity().z) * 0.75f * bulletTime;

							actualTargetPos += finalVelocity;

							Vector3 targetPosition = actualTargetPos;


							auto _aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);
							float bulletTime = 1337.f;
							int sims = 0;
							while (sims < 30)
							{

								sims++;
								bool hitPlayer = false;

								_aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);
								Vector3 velocity = _aimDir * projectileVelocity * projectileVelocityScale;

								auto currentPosition = localPos;
								auto previousPosition = currentPosition;

								Vector3 closestPoint(FLT_MAX, FLT_MAX, FLT_MAX);
								Vector3 offset = Vector3().Zero();

								for (int i = -1; i < num3; i++)
								{
									previousPosition = currentPosition;
									currentPosition += velocity * num;
									velocity += gravity * gravityModifier * num;
									velocity -= velocity * drag * num;

									auto line = (currentPosition - previousPosition);
									auto len = line.UnityMagnitude();
									line.UnityNormalize();
									auto v = actualTargetPos - previousPosition;
									auto d = Vector3().UnityDot(v, line);

									if (d < 0.f)
									{
										d = 0.f;
									}
									else if (d > len)
									{
										d = len;
									}

									Vector3 nearestPoint = previousPosition + line * d;

									if (nearestPoint.distance(actualTargetPos) < 0.1f)
									{
										bulletTime = i * num;
										hitPlayer = true;
									}
									else if (nearestPoint.distance(actualTargetPos) < closestPoint.distance(actualTargetPos))
									{
										closestPoint = nearestPoint;
										offset = actualTargetPos - nearestPoint;
									}
								}

								if (hitPlayer) break;
								targetPosition += offset;
							}

							did_reload = false;
							just_shot = true;
							fixed_time_last_shot = Time::fixedTime();

							return _aimDir;
						}
					}
				}
			if (vars::weapons::no_spread) {
				aimCone = 0.f;
			}
			did_reload = false;
			just_shot = true;
			fixed_time_last_shot = Time::fixedTime();
			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			return original_aimconedirection(aimCone, inputVec, anywhereInside);
		}
		Vector3 hk_BodyLeanOffset(PlayerEyes* a1) {
			/*if (vars::combat::manipulator && !other::m_manipulate.empty()) {
				return other::m_manipulate;
			}*/
			return Original_BodyLeanOffset(a1);
		}
		Vector3 hk_EyePositionForPlayer(BaseMountable* arg1, BasePlayer* arg2, Quaternion* arg3) {
			BasePlayer* player = arg2;
			if (player->userID()) {
				if (vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key)) {
					return Original_EyePositionForPlayer(arg1, arg2, arg3) /*+ other::m_manipulate*/;
				}
			}
			return Original_EyePositionForPlayer(arg1, arg2, arg3);
		}
		void hk_DoFirstPersonCamera(PlayerEyes* a1, Component* cam) {
			if (!a1 || !cam) return;
			Original_DoFirstPersonCamera_hk(a1, cam);
			if (vars::combat::manipulator) {
				Vector3 re_p = LocalPlayer::Entity()->transform()->position() + LocalPlayer::Entity()->transform()->up() * (PlayerEyes::EyeOffset().y + LocalPlayer::Entity()->eyes()->viewOffset().y);
				cam->transform()->set_position(re_p);
			}
		}
	}
	System::Object_* StartCoroutine_hk(MonoBehaviour* a1, System::Object_* un2) {
		if (vars::misc::fast_loot) {
			static auto v = METHOD("Assembly-CSharp::ItemIcon::SetTimedLootAction(UInt32,Action): Void");
			if (CALLED_BY(v, 0x656)) {
				*reinterpret_cast<float*>(un2 + 0x28) = -0.2f;
			}
		}
		return a1->StartCoroutine(un2);
	}
}

void UpdateVelocity_hk(PlayerWalkMovement* self) {
	if (vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key)) {

		self->TargetMovement() = Vector3::Zero();
		self->jumpTime() = 0.f;
		self->grounded() = true;
	
		LocalPlayer::Entity()->movement()->TeleportTo(LocalPlayer::Entity()->lastSentTick()->__position());
	}
	if (GetAsyncKeyState(vars::keys::paste)) {
		self->flying() = true;
		Vector3 targetmovement{};

		Quaternion rotation = LocalPlayer::Entity()->eyes()->rotation();

		if (GetAsyncKeyState('W') & 1)
			targetmovement += Vector3(0, -rotation.y, rotation.z);
		if (GetAsyncKeyState('S') & 1)
			targetmovement -= Vector3(0, rotation.y, rotation.z);
		if (GetAsyncKeyState('A') & 1)
			targetmovement -= Vector3(rotation.x, 0, 0);
		if (GetAsyncKeyState('D') & 1)
			targetmovement += Vector3(rotation.x, 0, 0);

		if (GetAsyncKeyState(VK_SPACE))
			targetmovement.y += 1.f;

		float d = 5.f;
		if (GetAsyncKeyState(VK_CONTROL))
			d = 2.5f;
		if (GetAsyncKeyState(VK_SHIFT))
			d = 10.f;

		if (!(targetmovement == Vector3{ 0.f, 0.f, 0.f }))
			self->TargetMovement() = targetmovement * d;
	}
	self->capsule()->set_radius(0.43);

	if (!self->flying()) {
		Vector3 vel = self->TargetMovement();
		if (vars::misc::omnidirectional_sprinting) {
			float max_speed = (self->swimming() || self->Ducking() > 0.5) ? 1.7f : 5.5f;
			if (vel.length() > 0.f) {

				Vector3 target_vel = Vector3(vel.x / vel.length() * max_speed, vel.y, vel.z / vel.length() * max_speed);
				self->TargetMovement() = target_vel;
			}
		}
	}

	return original_updatevelos(self);
}

void HandleJumping_hk(PlayerWalkMovement* a1, ModelState* state, bool wantsJump, bool jumpInDirection = false) {
	if (vars::misc::inf_jump) {
		if (!wantsJump)
			return;

		a1->grounded() = (a1->climbing() = (a1->sliding() = false));
		state->set_ducked(false);
		a1->jumping() = true;
		state->set_jumped(true);
		a1->jumpTime() = Time::time();
		a1->ladder() = nullptr;

		Vector3 curVel = a1->body()->velocity();
		a1->body()->set_velocity({ curVel.x, 10, curVel.z });
		return;
	}

	return original_jumpup(a1, state, wantsJump, jumpInDirection);
}

inline float __fastcall Fake_GetSpeed(float* a1, float* a2)
{
	if (vars::misc::speedhack && GetAsyncKeyState(vars::keys::speedkey));
	return true;
	return  Orig_GetSpeed(a1, a2);
}

void OnLand_hk(BasePlayer* ply, float vel) {
	if (!LocalPlayer::Entity()->get_flag(PlayerFlags::Connected)) return ply->OnLand(vel);
	if (!vars::misc::no_fall)
		ply->OnLand(vel);
}

bool IsDown_hk(InputState* self, Button btn) {
	if (!LocalPlayer::Entity()->get_flag(PlayerFlags::Connected)) return original_isdown(self, btn);
	if (LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1588298435 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -778367295 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1367281941 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -765183617 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -41440462 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 795371088 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -75944661 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1965232394 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 884424049 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1443579727 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1318558775 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1796682209 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 649912614 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 818877484 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -852563019 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1373971859 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1758372725 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1812555177 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -904863145 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1545779598 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1335497659 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -2069578888 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == -1214542497 || LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 28201841 ||
		LocalPlayer::Entity()->GetActiveWeapon()->GetID() == 1953903201) {
		if (vars::combat::autoshoot) {
			if (vars::combat::desync_autoshoot_type == 0) {
				if (vars::combat::autoshoot_type == 1) {

					if (btn == Button::FIRE_PRIMARY) {
						auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
						if (held && !held->Empty() && held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
							if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->isCached()) {
								auto mpv = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->find_mpv_bone();
								Vector3 target;
								if (mpv != nullptr)
									target = mpv->position;
								else
									target = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);

								if (utils::LineOfSight(target, LocalPlayer::Entity()->eyes()->position()) && vars::combat::autoshoot)
									return true;
							}
						}
					}
				}
			}
		}
		if (vars::combat::desync_autoshoot_type == 1) {
			if (vars::combat::autoshoot && vars::combat::manipulator && GetAsyncKeyState(vars::keys::manipulated_key)) {
				if (vars::combat::autoshoot_type == 1) {
					if (btn == Button::FIRE_PRIMARY) {
						auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
						if (held && !held->Empty() && held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
							if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->isCached()) {
								auto mpv = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->find_mpv_bone();
								Vector3 target;
								if (mpv != nullptr)
									target = mpv->position;
								else
									target = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head);

								if (utils::LineOfSight(target, LocalPlayer::Entity()->eyes()->position()) && vars::combat::autoshoot)
									return true;
							}
						}
					}
				}
			}
		}

		float desyncpercentage;
		float desyncTime = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime()) - 0.03125 * 3;
		desyncpercentage = ((desyncTime / 0.99f) * 100.0f) + 1.f;
		if (!LocalPlayer::Entity()->get_flag(PlayerFlags::Connected)) return original_isdown(self, btn);
		if (vars::combat::instakill && GetAsyncKeyState(vars::keys::instakill)) {
			if (vars::combat::instakill_type == 1) {
				if (btn == Button::FIRE_PRIMARY) {
					auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
					if (held && !held->Empty() && held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
						if (reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer) != nullptr && !LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->userID()) && reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->IsValid() && LocalPlayer::Entity()->get_flag(PlayerFlags::Connected)/* && !did_reload*/) {
							if (utils::LineOfSight(reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer)->get_bone_pos(head), LocalPlayer::Entity()->eyes()->position())) {
								LocalPlayer::Entity()->clientTickInterval() = 0.99f;
								if (desyncTime > 0.80f) {
									return true;
								}
							}
						}
					}
				}
			}
		}
	}

	return original_isdown(self, btn);
}

void hk_(void* Function, void** Original, void* Detour) {
	if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED) {
		MessageBox(0, (L"Initialization hook error in product, go to 2iq fix pls. dm  to get help."), 0, 0);
		return;
	}
	MH_CreateHook(Function, Detour, Original);
	MH_EnableHook(Function);
}

void hk__() {

	hk_((void*)METHOD("Rust.Data::ModelState::set_flying(Boolean): Void"), (void**)&original_setflying, hk::misc::SetFlying);
	hk_((void*)(uintptr_t)(vars::stor::gBase + CO::SendProjectileAttack), (void**)&original_sendprojectileattack, hk::combat::SendProjectileAttack);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::CanAttack(): Boolean"), (void**)&original_canattack, hk::combat::CanAttack);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::SendClientTick(): Void"), (void**)&original_sendclienttick, hk::misc::SendClientTick);
	hk_((void*)METHOD("Assembly-CSharp::AimConeUtil::GetModifiedAimConeDirection(Single,Vector3,Boolean): Vector3"), (void**)&original_aimconedirection, hk::combat::GetModifiedAimConeDirection);
	hk_((void*)(uintptr_t)(vars::stor::gBase + CO::CanHoldItems), (void**)&original_canholditems, hk::combat::CanHoldItems);
	hk_((void*)METHOD("Facepunch.Console::ConsoleSystem::Run(Option,String,Object[]): String"), (void**)&original_consolerun, hk::misc::Run);
	hk_((void*)(uintptr_t)(vars::stor::gBase + CO::CreateProjectile), (void**)&original_create_projectile, hk::combat::CreateProjectile);
	hk_((void*)METHOD("Assembly-CSharp::EffectLibrary::CreateEffect(String,Effect): GameObject"), (void**)&original_createeffect, hk::misc::CreateEffect);
	hk_((void*)(uintptr_t)(vars::stor::gBase + CO::Play), (void**)&original_viewmodelplay, hk::misc::Play);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::VisUpdateUsingCulling(Single,Boolean): Void"), (void**)&original_UnregisterFromVisibility, hk::misc::VisUpdateUsingCulling);
	hk_((void*)METHOD("Assembly-CSharp::ItemModProjectile::GetRandomVelocity(): Single"), (void**)&original_getrandomvelocity, hk::combat::GetRandomVelocity);
	hk_((void*)METHOD("Assembly-CSharp::HeldEntity::AddPunch(Vector3,Single): Void"), (void**)&original_addpunch, hk::combat::AddPunch);
	hk_((void*)METHOD("UnityEngine.CoreModule::UnityEngine::Vector3::MoveTowards(Vector3,Vector3,Single): Vector3"), (void**)&original_movetowards, hk::combat::MoveTowards);
	hk_((void*)METHOD("Assembly-CSharp::Projectile::DoMovement(Single): Void"), (void**)&original_domovement, hk::exploit::DoMovement);
	hk_((void*)METHOD("Assembly-CSharp::Projectile::Launch(): Void"), (void**)&original_launch, hk::combat::Launch);
	hk_((void*)(uintptr_t)(vars::stor::gBase + CO::DoFixedUpdate), (void**)&original_dofixedupdate, hk::misc::DoFixedUpdate);
	hk_((void*)METHOD("Assembly-CSharp::Projectile::DoHit(HitTest,Vector3,Vector3): Boolean"), (void**)&original_dohit, hk::combat::DoHit);
	hk_((void*)METHOD("Assembly-CSharp-firstpass::TOD_Sky::UpdateAmbient(): Void"), (void**)&original_updateambient, hk::misc::UpdateAmbient);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::ClientInput(InputState): Void"), (void**)&original_clientinput, hk::misc::ClientInput);
	hk_((void*)METHOD("Assembly-CSharp::BaseCombatEntity::DoHitNotify(HitInfo): Void"), (void**)&original_dohitnotify, hk::misc::DoHitNotify);
	hk_((void*)(uintptr_t)(vars::stor::gBase + CO::get_isHeadshot), (void**)&original_getisheadshot, hk::misc::get_isHeadshot);
	hk_((void*)(uintptr_t)(vars::stor::gBase + CO::StartCoroutine), (void**)&MonoBehaviour::StartCoroutine_, hk::StartCoroutine_hk);
	hk_((void*)METHOD("Assembly-CSharp::PlayerWalkMovement::UpdateVelocity(): Void"), (void**)&original_updatevelos, UpdateVelocity_hk);
	hk_((void*)METHOD("Assembly-CSharp::PlayerWalkMovement::HandleJumping(ModelState,Boolean,Boolean): Void"), (void**)&original_jumpup, HandleJumping_hk);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::OnLand(Single): Void"), (void**)&BasePlayer::OnLand_, OnLand_hk);
	hk_((void*)METHOD("Assembly-CSharp::InputState::IsDown(BUTTON): Boolean"), (void**)&original_isdown, IsDown_hk);
	hk_((void*)METHOD("Assembly-CSharp::PlayerEyes::get_BodyLeanOffset(): Vector3"), (void**)&Original_BodyLeanOffset, hk::combat::hk_BodyLeanOffset);
	hk_((void*)METHOD("Assembly-CSharp::BaseMountable::EyePositionForPlayer(BasePlayer,Quaternion): Vector3"), (void**)&Original_EyePositionForPlayer, hk::combat::hk_EyePositionForPlayer);
	hk_((void*)METHOD("Assembly-CSharp::PlayerEyes::DoFirstPersonCamera(Camera): Void"), (void**)&Original_DoFirstPersonCamera_hk, hk::combat::hk_DoFirstPersonCamera);
}