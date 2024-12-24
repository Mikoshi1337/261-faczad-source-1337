void game_thread_loop() {
	DWORD64 BaseNetworkable;
	BaseNetworkable = read(vars::stor::gBase + CO::BaseNetworkable, DWORD64);

	if (!LocalPlayer::Entity()) return;

	auto entityList = BaseNetworkable::clientEntities()->entityList();
	if (entityList) {
		for (int i = 0; i < entityList->vals->size; i++) {
			uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			if (!Entity) continue;
			uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
			if (!Object) continue;
			uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
			if (!ObjectClass) continue;

			pUncStr name = read(ObjectClass + 0x60, pUncStr); if (!name) continue;
			char* buff = name->stub;

			if (!reinterpret_cast<BaseEntity*>(Entity)->IsValid( )) { continue; }
			if (!reinterpret_cast<Component*>(Entity)->gameObject( )) { continue; }
			if (strstr(buff, ("Local"))) { continue; }

			Item* weapon = LocalPlayer::Entity()->GetActiveWeapon();
			DWORD64 active = weapon->entity( );
			char* classname = weapon->ClassName();

			bool weaponmelee = weapon && classname && (strcmp(classname, ("BaseMelee")) || strcmp(classname, ("Jackhammer")));
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer()) {
				BasePlayer* lol = (BasePlayer*)Entity;

				if (!lol) continue;

				if (vars::misc::auto_revive) {
					UINT64 gameObject = read(ObjectClass + 0x30, UINT64);
					Vector3 local = utils::ClosestPoint(LocalPlayer::Entity(), utils::GetEntityPosition(gameObject));
					if (vars::misc::auto_revive && (BasePlayer*)Entity && lol->get_flag(PlayerFlags::Wounded) && Math::Distance_3D(local, utils::GetEntityPosition(gameObject)) < 3.f && LocalPlayer::Entity()->GetKeyState(Button::USE)) {
						lol::pickup_player((BasePlayer*)Entity);
					}
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
	}
}