
enum weapon_config_struct
{
	w_model_id,
	w_name[64]
}

new const weapon_config[][weapon_config_struct] = {
	{WEAPON_BRASSKNUCKLE, "Кастет"},
	{WEAPON_GOLFCLUB, "Клюшка для гольфа"},
	{WEAPON_NITESTICK, "Дубинка"},
	{WEAPON_KNIFE, "Нож"},
	{WEAPON_BAT, "Бейсбольная бита"},
	{WEAPON_SHOVEL, "Лопата"},
	{WEAPON_POOLSTICK, "Бильярдный кий"},
	{WEAPON_KATANA, "Катана"},
	{WEAPON_CHAINSAW, "Бензопила"},
	{WEAPON_FLOWER, "Цветы"},
	{WEAPON_COLT45, "Пистолет Glock-17"},
	{WEAPON_SILENCED, "Пистолет M1911 Silenced"},
	{WEAPON_DEAGLE, "Пистолет Beretta M9"},
	{WEAPON_SHOTGUN, "Дробовик Remington 870"},
	{WEAPON_MP5, "Пистолет-пулемёт MP5"},
	{WEAPON_AK47, "Автомат АК-47"},
	{WEAPON_M4, "Автомат М4"},
	{WEAPON_RIFLE, "Винтовка Winchester 1873"}
};

new const WEAPON_CONFIG_SIZE = sizeof(weapon_config);

stock GetWeaponConfigIndex(weapon_id){
	for(new i; i < WEAPON_CONFIG_SIZE; i++){
		if(weapon_config[i][w_model_id] == weapon_id) return i;
	}
	return -1;
}