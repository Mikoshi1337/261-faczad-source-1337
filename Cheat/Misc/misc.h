void weapon_set( ) {
	Item* weapon = LocalPlayer::Entity( )->GetActiveWeapon( );
	if (weapon == nullptr) return;

	//-3 - hammer, -2 - bows, -1 - eoka, 0-nopatch, 1 - meele, 2 - semiautomatic, 3 - automatic
	Weapon target = weapon->Info( );
	if (!target.id) return;
	if (!target.category) return;
	if (target.category == -2) {
		weapon->NoSway( );
		weapon->FastBow( );
		return;
	}
	if (target.category == 3) {
		weapon->NoSway( );
		weapon->RapidFire( );
		weapon->instakill_button();
		return;
	}
	if (target.category == 2) {
		weapon->NoSway( );
		weapon->instakill_button( );
		weapon->SetAutomatic( );
		return;
	}
	if (target.category == -1) {
		weapon->NoSway( );
		weapon->EokaTap( );
		return;
	}
}
float w_last_syringe = 0.f;
void misc_set( ) {
	if (w_last_syringe == 0.f) {
		w_last_syringe = LocalPlayer::Entity()->lastSentTickTime();
	}
	Item* weapon = LocalPlayer::Entity( )->GetActiveWeapon( );
	if (vars::misc::faster_healing && (weapon->info_()->itemid() == 1079279582 || weapon->info_()->itemid() == -2072273936) && LocalPlayer::Entity()->health() < 99) {
		BaseProjectile* ent = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
		if (LocalPlayer::Entity()->lastSentTickTime() > w_last_syringe + 0.7f) {
			ent->_ServerRPC(("UseSelf"));
			w_last_syringe = LocalPlayer::Entity()->lastSentTickTime();
		}
	}

	float changeSpeed = 0.2f;
	float FOV = vars::misc::fov;

	if (vars::visuals::zoom && GetAsyncKeyState(vars::keys::zoom))
	{
		float oldFOV = ConVar::Graphics::_fov();
		float newFOV = oldFOV + (15.f - oldFOV) * changeSpeed;
		ConVar::Graphics::_fov() = newFOV;
	}
	else
	{
		float zoomfov = ConVar::Graphics::_fov();
		float newFOV = zoomfov + (FOV - zoomfov) * changeSpeed;
		ConVar::Graphics::_fov() = newFOV;
	}

	if (vars::misc::spiderman) {
		uintptr_t Movement = read(LocalPlayer::Entity( ) + O::BasePlayer::movement, uintptr_t);
		write(Movement + O::PlayerWalkMovement::groundAngle, 0.f, float);
		write(Movement + O::PlayerWalkMovement::groundAngleNew, 0.f, float);
	}
	if (vars::misc::fakeadmin)
		LocalPlayer::Entity( )->FakeAdmin( );
	if (vars::misc::custom_time)
		LocalPlayer::Entity()->Admintime();
}

namespace lol {
	void do_attack(f_object target, uintptr_t Held, bool transform) {
		if (!target.valid || !Held) return;

		if (read(Held + O::AttackEntity::nextAttackTime, float) >= Time::time( )) { return; }
		if (read(Held + O::AttackEntity::timeSinceDeploy, float) < read(Held + O::AttackEntity::deployDelay, float)) { return; }

		uintptr_t staticHitTest = read(vars::stor::gBase + CO::HitTest, DWORD64); if (!staticHitTest) return;
		uintptr_t newHitTest = il2cpp::il2cpp_object_new(staticHitTest); if (!newHitTest) return;

		uintptr_t trans; Ray ray = Ray(LocalPlayer::Entity( )->get_bone_pos(neck), (target.position - LocalPlayer::Entity( )->get_bone_pos(neck)).Normalized( ));
		if (!target.entity) return;
		if (transform) {
			trans = reinterpret_cast<BasePlayer*>(target.entity)->mono_transform(head);
		}
		else {
			trans = (DWORD64)reinterpret_cast<Object*>(target.entity)->transform( );
		} if (!trans) {
		//	LogSystem::Log((L"Transform no found in Infinity product, go to update mono_transform pls. dm oxunem#1337 to get help."), 1.f);
			return;
		}

		write(newHitTest + O::HitTest::MaxDistance, 1000.f, float);
		write(newHitTest + O::HitTest::HitTransform, trans, uintptr_t);
		write(newHitTest + O::HitTest::AttackRay, ray, Ray);
		write(newHitTest + O::HitTest::DidHit, true, bool);
		write(newHitTest + O::HitTest::HitEntity, reinterpret_cast<BasePlayer*>(target.entity), BasePlayer*);
		write(newHitTest + O::HitTest::HitPoint, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + O::HitTest::HitNormal, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + O::HitTest::damageProperties, read(Held + O::BaseMelee::damageProperties, uintptr_t), uintptr_t);
		reinterpret_cast<void(*)(uintptr_t, float)>(vars::stor::gBase + CO::StartAttackCooldown)(Held, read(Held + O::AttackEntity::repeatDelay, float));
		return reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + CO::ProcessAttack)(Held, newHitTest);
	}

	void hammerspam_attack(f_object target, uintptr_t Held, bool transform) {
		if (!target.valid || !Held) return;

		if (read(Held + O::AttackEntity::nextAttackTime, float) >= Time::time()) { return; }
		if (read(Held + O::AttackEntity::timeSinceDeploy, float) < read(Held + O::AttackEntity::deployDelay, float)) { return; }

		uintptr_t staticHitTest = read(vars::stor::gBase + CO::HitTest, DWORD64); if (!staticHitTest) return;
		uintptr_t newHitTest = il2cpp::il2cpp_object_new(staticHitTest); if (!newHitTest) return;

		uintptr_t trans; Ray ray = Ray(LocalPlayer::Entity()->get_bone_pos(head), (target.position - LocalPlayer::Entity()->get_bone_pos(head)).Normalized());
		if (!target.entity) return;
		if (transform) {
			trans = reinterpret_cast<BasePlayer*>(target.entity)->mono_transform(head);
		}
		else {
			trans = (DWORD64)reinterpret_cast<Object*>(target.entity)->transform();
		} if (!trans) {
			return;
		}

		write(newHitTest + O::HitTest::MaxDistance, 1000.f, float);
		write(newHitTest + O::HitTest::HitTransform, trans, uintptr_t);
		write(newHitTest + O::HitTest::AttackRay, ray, Ray);
		write(newHitTest + O::HitTest::DidHit, true, bool);
		write(newHitTest + O::HitTest::HitEntity, reinterpret_cast<BasePlayer*>(target.entity), BasePlayer*);
		write(newHitTest + O::HitTest::HitPoint, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + O::HitTest::HitNormal, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + O::HitTest::damageProperties, read(Held + O::BaseMelee::damageProperties, uintptr_t), uintptr_t);

		if (vars::combat::ishak == 0) {
			write(newHitTest + O::HitTest::HitMaterial, il2cpp::String::New(("glass")), il2cpp::String*);
		}
		else {
			write(newHitTest + O::HitTest::HitMaterial, il2cpp::String::New(("water")), il2cpp::String*);
		}

		return reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + CO::ProcessAttack)(Held, newHitTest);
	}

	void hammerspam1_attack(f_object target, uintptr_t Held, bool transform) {
		if (!target.valid || !Held) return;

		if (read(Held + O::AttackEntity::nextAttackTime, float) >= Time::time()) { return; }
		if (read(Held + O::AttackEntity::timeSinceDeploy, float) < read(Held + O::AttackEntity::deployDelay, float)) { return; }

		uintptr_t staticHitTest = read(vars::stor::gBase + CO::HitTest, DWORD64); if (!staticHitTest) return;
		uintptr_t newHitTest = il2cpp::il2cpp_object_new(staticHitTest); if (!newHitTest) return;

		uintptr_t trans; Ray ray = Ray(LocalPlayer::Entity()->get_bone_pos(head), (target.position - LocalPlayer::Entity()->get_bone_pos(head)).Normalized());
		if (!target.entity) return;
		if (transform) {
			trans = reinterpret_cast<BasePlayer*>(target.entity)->mono_transform(head);
		}
		else {
			trans = (DWORD64)reinterpret_cast<Object*>(target.entity)->transform();
		} if (!trans) {
			return;
		}

		write(newHitTest + O::HitTest::MaxDistance, 1000.f, float);
		write(newHitTest + O::HitTest::HitTransform, trans, uintptr_t);
		write(newHitTest + O::HitTest::AttackRay, ray, Ray);
		write(newHitTest + O::HitTest::DidHit, true, bool);
		write(newHitTest + O::HitTest::HitEntity, reinterpret_cast<BasePlayer*>(target.entity), BasePlayer*);
		write(newHitTest + O::HitTest::HitPoint, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + O::HitTest::HitNormal, reinterpret_cast<Transform*>(trans)->InverseTransformPoint(target.position), Vector3);
		write(newHitTest + O::HitTest::damageProperties, read(Held + O::BaseMelee::damageProperties, uintptr_t), uintptr_t);

		if (vars::combat::ishak == 0) {
			write(newHitTest + O::HitTest::HitMaterial, il2cpp::String::New(("glass")), il2cpp::String*);
		}
		else {
			write(newHitTest + O::HitTest::HitMaterial, il2cpp::String::New(("water")), il2cpp::String*);
		}
		
		reinterpret_cast<void(*)(uintptr_t, float)>(vars::stor::gBase + CO::StartAttackCooldown)(Held, read(Held + O::AttackEntity::repeatDelay, float));

		return reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(vars::stor::gBase + CO::ProcessAttack)(Held, newHitTest);
	}

	// 0, -9.1 * gravityModifier, 0
	TraceResult traceProjectile(Vector3 position, Vector3 velocity, float drag, Vector3 gravity, Vector3 targetPoint) {
		constexpr float num = 0.03125f;
		Vector3 prevPosition = position;
		float prevDist = FLT_MAX;
		Line resultLine = Line(position, position);
		float travelTime = 0.f;
		TraceResult result;

		for (; travelTime < 8.f; ) {
			prevPosition = position;
			position += velocity * num;

			Line line = Line(prevPosition, position);
			Vector3 nearest = line.ClosestPoint(targetPoint);

			float dst = (nearest - targetPoint).Length( );

			if (dst > prevDist) {
				break;
			}
			prevDist = dst;
			resultLine = line;

			velocity += gravity * num;
			velocity -= velocity * drag * num;
			travelTime += num;
		}

		Vector3 hitPos = resultLine.ClosestPoint(targetPoint);

		result.hitDist = (hitPos - targetPoint).Length( );
		result.hitPosition = hitPos;
		result.outVelocity = velocity;
		result.hitTime = travelTime - num;
		return result;
	};
	float LastUpdate = 0.f;
	float LastGrade = 0.f;
	float LastKnock = 0.f; float LastOpen = 0.f; float LastHatch = 0.f;
	float LastPickup = 0.f;
	void pickup_player(BasePlayer* ent) {
		typedef void(__stdcall* AssistPlayer)(BasePlayer*, BasePlayer*);
		if (!LocalPlayer::Entity( )->is_teammate(ent->userID( )) && vars::misc::revive_team_only) return;
		if (LocalPlayer::Entity( )->lastSentTickTime( ) > LastPickup + 0.5f) {
			((AssistPlayer)(vars::stor::gBase + CO::AssistPlayer))(ent, LocalPlayer::Entity( ));
			LastPickup = LocalPlayer::Entity( )->lastSentTickTime( );
		}
	}
	void auto_farm_loop(bool weaponmelee, uintptr_t active) {
		if (vars::misc::auto_pickup) {
			f_object entity = f_object::get_closest_object(LocalPlayer::Entity( )->get_bone_pos(head),
				("/collectable/"),
				Vector3::Zero( ),
				Vector3::Zero( ),
				Vector3::Zero( ),
				false
			);
			if (entity.valid) {
				Vector3 local = utils::ClosestPoint(LocalPlayer::Entity( ), entity.position);
				if (Math::Distance_3D(local, entity.position) < 3.f) {
					if (reinterpret_cast<BaseEntity*>(entity.entity)->IsValid( )) {
						utils::ServerRPC(entity.entity, Str((L"Pickup")));
					}
				}
			}
		}
		if (vars::misc::auto_farm) {
			if (vars::misc::auto_farm_barrel || vars::misc::farmbot_barrels) {
				if (weaponmelee) {
					f_object ore_hot_spot = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head),
						("barrel"),
						Vector3::Zero(),
						Vector3::Zero(),
						Vector3::Zero(),
						false,
						(""));
					if (ore_hot_spot.valid) {
						Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), ore_hot_spot.position);
						if (Math::Distance_3D(local, ore_hot_spot.position) <= 2.f) {
							if (reinterpret_cast<BaseEntity*>(ore_hot_spot.entity)->IsValid()) {
								lol::do_attack(ore_hot_spot, active, false);
							}
						}
					}
				}
			}
			if (vars::misc::auto_farm_ore || vars::misc::farmbot_ore) {
				if (weaponmelee) {
					f_object ore_hot_spot = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head),
						(""),
						Vector3::Zero(),
						Vector3::Zero(),
						Vector3::Zero(),
						true,
						("OreHotSpot"));
					if (ore_hot_spot.valid) {
						Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), ore_hot_spot.position);
						if (Math::Distance_3D(local, ore_hot_spot.position) <= 2.f) {
							if (reinterpret_cast<BaseEntity*>(ore_hot_spot.entity)->IsValid()) {
								lol::do_attack(ore_hot_spot, active, false);
							}
						}
					}
				}
			}
			if (vars::misc::auto_farm_tree) {
				if (weaponmelee) {
					f_object tree_entity = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head),
						(""),
						Vector3::Zero(),
						Vector3::Zero(),
						Vector3::Zero(),
						true,
						("TreeEntity"));
					if (tree_entity.valid) {
						tree_entity.position = Vector3::Zero();
						f_object tree_marker = f_object::get_closest_object(LocalPlayer::Entity()->get_bone_pos(head),
							(""),
							Vector3::Zero(),
							Vector3::Zero(),
							Vector3::Zero(),
							true,
							("TreeMarker"));
						if (tree_marker.valid) {
							Vector3 locala = utils::ClosestPoint(LocalPlayer::Entity(), tree_marker.position);
							if (Math::Distance_3D(locala, tree_marker.position) <= 2.f) {
								tree_entity.position = tree_marker.position;
								Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), tree_entity.position);
								if (Math::Distance_3D(local, tree_entity.position) <= 2.f) {
									if (reinterpret_cast<BaseEntity*>(tree_entity.entity)->IsValid()) {
										lol::do_attack(tree_entity, active, false);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
void TestFlying() {
	flyhackPauseTime = Mathf::Max(0.f, flyhackPauseTime - Time::deltaTime());
	bool inAir = false;
	float radius = LocalPlayer::Entity()->GetRadius();
	float height = LocalPlayer::Entity()->GetHeight(false);
	Vector3 vector = (LocalPlayer::Entity()->lastSentTick()->__position() + LocalPlayer::Entity()->transform()->position()) * 0.5f;
	Vector3 vector2 = vector + Vector3(0.f, radius - 1.85f, 0.f);
	Vector3 vector3 = vector + Vector3(0.f, height - radius, 0.f);
	float radius2 = radius - 0.05f;
	bool a = Physics::CheckCapsule(vector2, vector3, radius2, 1503731969, QueryTriggerInteraction::Ignore);
	inAir = !a;

	if (inAir) {
		bool flag = false;

		Vector3 vector4 = (LocalPlayer::Entity()->transform()->position() - LocalPlayer::Entity()->lastSentTick()->__position());
		float num3 = Mathf::Abs(vector4.y);
		float num4 = Misc::Magnitude2D(vector4);

		if (vector4.y >= 0.f) {
			flyhackDistanceVertical += vector4.y;
			flag = true;
		}
		if (num3 < num4) {
			flyhackDistanceHorizontal += num4;
			flag = true;
		}
		float flyhack_forgiveness_interia = 10.0f;
		float flyhack_forgiveness = 1.5f;
		float flyhack_extrusion = 1.85f;
		float num5 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
		float num6 = ((LocalPlayer::Entity()->GetJumpHeight() + num5) * 3);

		VMaxFlyhack = num6;
		if (flyhackDistanceVertical <= (num6)) {
			VFlyhack = flyhackDistanceVertical;
		}
		if (VFlyhack >= VMaxFlyhack)
			VFlyhack = VMaxFlyhack;
		float num7 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
		float num8 = ((5.f + num7) * 3);

		HMaxFlyhack = num8;
		if (flyhackDistanceHorizontal <= (num8)) {
			HFlyhack = flyhackDistanceHorizontal;
		}
		if (HFlyhack >= HMaxFlyhack)
			HFlyhack = HMaxFlyhack;
		if (flag) {
			float num5 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
			float num6 = LocalPlayer::Entity()->GetJumpHeight() + num5;
			if (flyhackDistanceVertical > num6) {
			}
			float num7 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
			float num8 = 5.f + num7;
			if (flyhackDistanceHorizontal > num8) {
			}
		}
	}
	else {
		flyhackDistanceHorizontal = 0.f;
		flyhackDistanceVertical = 0.f;
	}
}
void CheckFlyhack( ) {
	TestFlying();

	float xs = vars::stuff::ScreenWidth / 2, ys = vars::stuff::ScreenHeight / 2;

	float num5 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num6 = LocalPlayer::Entity( )->GetJumpHeight( ) + num5;
	vars::stuff::max_flyhack = num6;
	if (flyhackDistanceVertical <= num6) {
		vars::stuff::flyhack = flyhackDistanceVertical;
	}

	float num7 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num8 = 5.f + num7;
	vars::stuff::max_hor_flyhack = num8;
	if (flyhackDistanceHorizontal <= num8) {
		vars::stuff::hor_flyhack = flyhackDistanceHorizontal;
	}

	//if (vars::misc::flyhack_indicator && (flyhackDistanceVertical > num6)) {
	//	g_Render->DrawString(xs, ys - 656, ImColor(255, 0, 0), render2::outline | render2::centered_x, pixel_s, 11, ("can kick"));
	//}
	//if (vars::misc::flyhack_indicator && (flyhackDistanceHorizontal > num6)) {
	//	g_Render->DrawString(xs, ys - 643, ImColor(255, 0, 0), render2::outline | render2::centered_x, pixel_s, 11, ("can kick"));
	//}
}

auto* TargetPlayerA = reinterpret_cast<BasePlayer*>(vars::stor::closestPlayer);