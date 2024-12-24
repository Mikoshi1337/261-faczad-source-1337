struct Ammo {
	int id[2];
	float speed;
};
struct Weapon {
	int id;
	const char* name;
	Ammo ammo[4];
	int category; //-3 - hammer, -2 - bows, -1 - eoka, 0-nopatch, 1 - meele, 2 - semiautomatic, 3 - automatic
};
Weapon info[]{
Weapon{1588298435, ("Bolt Action Rifle"), { Ammo{{-1211166256}/*normal 5.56*/, 375.f}, Ammo{{1712070256} /*HV 5.56*/, 788 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 394}}, 2 },
Weapon{-778367295, ("L96 Rifle"), { Ammo{{-1211166256}/*normal 5.56*/, 625.f}, Ammo{{1712070256} /*HV 5.56*/, 1350 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 675}}, 2 },

Weapon{-1367281941, ("Waterpipe Shotgun"), { Ammo{{-727717969, -1685290200}/*green, red*/, 185.f}, Ammo{{588596902, -1036635990}/*gray, blue*/, 80}}, 2 },
Weapon{-765183617, ("Double Barrel Shotgun"), { Ammo{{-727717969, -1685290200}/*green, red*/, 185.f}, Ammo{{588596902, -1036635990}/*gray, blue*/, 80}}, 2 },
Weapon{-41440462, ("Spas-12 Shotgun"), { Ammo{{-727717969, -1685290200}/*green, red*/, 185.f}, Ammo{{588596902, -1036635990}/*gray, blue*/, 80}}, 2 },
Weapon{795371088, ("Pump Shotgun"), { Ammo{{-727717969, -1685290200}/*green, red*/, 185.f}, Ammo{{588596902, -1036635990}/*gray, blue*/, 80}}, 2 },
Weapon{-75944661, ("Eoka Pistol"), { Ammo{{-727717969, -1685290200}/*green, red*/, 185.f}, Ammo{{588596902, -1036635990}/*gray, blue*/, 80}}, -1 },

Weapon{1965232394, ("Crossbow"), { Ammo{{-1234735557}/*normal arrow*/, 78.f}, Ammo{{14241751} /*fire arrow*/, 61.f}, Ammo{{215754713} /*bone arrow*/, 66.f}, Ammo{{-1023065463} /*HV arrow*/, 120.f}}, 2 },
Weapon{884424049, ("Compound Bow"), { Ammo{{-1234735557}/*normal arrow*/, 50.f}, Ammo{{14241751} /*fire arrow*/, 40}, Ammo{{215754713} /*bone arrow*/, 45}, Ammo{{-1023065463} /*HV arrow*/, 78.8f}}, 2 }, //TODO on zoom velocity x2
Weapon{1443579727, ("Hunting Bow"), { Ammo{{-1234735557}/*normal arrow*/, 50.f}, Ammo{{14241751} /*fire arrow*/, 40}, Ammo{{215754713} /*bone arrow*/, 45}, Ammo{{-1023065463} /*HV arrow*/, 80} }, -2 },

Weapon{1318558775, ("MP5A4"), { Ammo{{785728077}/*normal pistol*/, 190.f}, Ammo{{-1691396643} /*HV pistol*/, 290.f}, Ammo{{51984655} /*inc pistol*/, 145.f}}, 3 },
Weapon{1796682209, ("Custom SMG"), { Ammo{{785728077}/*normal pistol*/, 205.f}, Ammo{{-1691396643} /*HV pistol*/, 300.f}, Ammo{{51984655} /*inc pistol*/, 145.f}}, 3 },
Weapon{649912614, ("Revolver"), { Ammo{{785728077}/*normal pistol*/, 300}, Ammo{{-1691396643} /*HV pistol*/, 400}, Ammo{{51984655} /*inc pistol*/, 225}}, 2 },
Weapon{818877484, ("Semi-Automatic Pistol"), { Ammo{{785728077}/*normal pistol*/, 300}, Ammo{{-1691396643} /*HV pistol*/, 400}, Ammo{{51984655} /*inc pistol*/, 225}}, 2 },
Weapon{-852563019, ("M92 Pistol"), { Ammo{{785728077}/*normal pistol*/, 300}, Ammo{{-1691396643} /*HV pistol*/, 400}, Ammo{{51984655} /*inc pistol*/, 225}}, 2 },
Weapon{1373971859, ("Python Revolver"), { Ammo{{785728077}/*normal pistol*/, 300}, Ammo{{-1691396643} /*HV pistol*/, 400}, Ammo{{51984655} /*inc pistol 225*/, 225}}, 2 },
Weapon{-1758372725, ("Thompson"), { Ammo{{785728077}/*normal pistol*/, 300}, Ammo{{-1691396643} /*HV pistol*/, 400}, Ammo{{51984655} /*inc pistol*/, 225}}, 3 },

Weapon{-1812555177, ("LR-300 Assault Rifle"), { Ammo{{-1211166256}/*normal 5.56*/, 375}, Ammo{{1712070256} /*HV 5.56*/, 450 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 225}}, 3 },
Weapon{-904863145, ("Semi-Automatic Rifle"), { Ammo{{-1211166256}/*normal 5.56*/, 375}, Ammo{{1712070256} /*HV 5.56*/, 450 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 225}}, 2 },
Weapon{1545779598, ("Assault Rifle"), { Ammo{{-1211166256}/*normal 5.56*/, 375}, Ammo{{1712070256} /*HV 5.56*/, 450 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 225}}, 3 },
Weapon{-1335497659, ("Assault Rifle Ice"), { Ammo{{-1211166256}/*normal 5.56*/, 375}, Ammo{{1712070256} /*HV 5.56*/, 450 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 225}}, 3 },
Weapon{-2069578888, ("M249"), { Ammo{{-1211166256}/*normal 5.56*/, 375.f}, Ammo{{1712070256} /*HV 5.56*/, 585 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 293}}, 3 },
Weapon{-1214542497, ("HMLMG"), { Ammo{{-1211166256}/*normal 5.56*/, 375.f}, Ammo{{1712070256} /*HV 5.56*/, 585 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 293}}, 3 },
Weapon{28201841, ("M39 Rifle"), { Ammo{{-1211166256}/*normal 5.56*/, 469}, Ammo{{1712070256} /*HV 5.56*/, 563 }, Ammo{{-1321651331, 605467368}/*inc, ex 5.56*/, 281}}, 2 },

Weapon{1090916276, ("Pitchfork"), {Ammo{{0}, 25}}, 1 },
Weapon{1540934679, ("Wooden Spear"), {Ammo{{0}, 25}}, 1 },
Weapon{1814288539, ("Bone Knife"), {Ammo{{0}, 20}}, 1 },
Weapon{-363689972, ("Snowball"), {Ammo{{0}, 20}}, 1 },
Weapon{1789825282, ("Candy Cane Club"), {Ammo{{0}, 20}}, 1 },
Weapon{-1137865085, ("Machete"), {Ammo{{0}, 20}}, 1 },
Weapon{1711033574, ("Bone Club"), {Ammo{{0}, 20}}, 1 },
Weapon{-1966748496, ("Mace"), {Ammo{{0}, 18}}, 1 },
Weapon{-194509282, ("Butcher Knife"), {Ammo{{0}, 20}}, 1 },
Weapon{1326180354, ("Salvaged Sword"), {Ammo{{0}, 20}}, 1 },
Weapon{-1469578201, ("Longsword"), {Ammo{{0}, 18}}, 1 },
Weapon{-1978999529, ("Salvaged Cleaver"), {Ammo{{0}, 18}}, 1 },
Weapon{1602646136, ("Stone Spear"), {Ammo{{0}, 30}}, 1 },
Weapon{2040726127, ("Combat Knife"), {Ammo{{0}, 30}}, 1 },
Weapon{963906841, ("Rock"), {Ammo{{0}, 18}}, 1 },

Weapon{1953903201, ("Nailgun"), { Ammo{{-2097376851}/*nails*/, 50}}, 2 },
Weapon{-1123473824, ("Multiple Grenade Launcher"), { Ammo{{915408809, 349762871}/*smoke, exp*/, 100}, Ammo{{1055319033}, 225}/*shotgun*/}, 2 },
Weapon{442886268, ("Rocket Launcher"), 0, 69 },
Weapon{-1215753368, ("Flame Thrower"), 0, 0 },
Weapon{200773292, ("Hammer"), {0}, -3 }
};