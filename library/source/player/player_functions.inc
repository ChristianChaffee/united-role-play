
stock SetPlayerSpawn(playerid, spawn_type){
	
	new spawn_vw, spawn_int;
	switch(spawn_type){
		case SPAWN_TYPE_STANDART:{
			new spawn_pos_index = random(sizeof(standart_spawn_pos));

			SetSpawnInfo(playerid, 0, GetPlayerData(playerid, p_skin), 
				standart_spawn_pos[spawn_pos_index][0], 
				standart_spawn_pos[spawn_pos_index][1], 
				standart_spawn_pos[spawn_pos_index][2], 
				standart_spawn_pos[spawn_pos_index][3], 
				0, 0, 0, 0, 0, 0);
			
			spawn_vw = GAME_VW;
			spawn_int = GAME_INT;
		}
	}

	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, GetPlayerData(playerid, p_money));

	SetPlayerScore(playerid, GetPlayerData(playerid, p_level));
	SetPlayerHealth(playerid, GetPlayerData(playerid, p_health));
	SetPlayerDrunkLevel(playerid, GetPlayerData(playerid, p_drunked_level));

	CheckPlayerLicenses(playerid);

	player_logged[playerid] = true;
	SpawnPlayer(playerid);

	SetPlayerVirtualWorld(playerid, spawn_vw);
	SetPlayerInterior(playerid, spawn_int);
	SetCameraBehindPlayer(playerid);
	TogglePlayerControllable(playerid, true);

	GivePlayerAllWeapons(playerid);
	SetPlayerColor(playerid, GetPlayerColorEx(playerid));

	CheckPlayerNotifications(playerid);
	return true;
}

stock LoadPlayer(playerid, Cache:cache_id){
	new temp_string[128];
	cache_set_active(cache_id);

	cache_get_value_name_int(0, "id", GetPlayerData(playerid, p_id));
	cache_get_value_name(0, "name", GetPlayerData(playerid, p_name));
	cache_get_value_name(0, "email", GetPlayerData(playerid, p_email));
	cache_get_value_name_int(0, "nationality", GetPlayerData(playerid, p_nationality));
	cache_get_value_name_int(0, "age", GetPlayerData(playerid, p_age));
	cache_get_value_name_int(0, "skin", GetPlayerData(playerid, p_skin));
	cache_get_value_name_int(0, "sex", GetPlayerData(playerid, p_sex));
	cache_get_value_name_int(0, "money", GetPlayerData(playerid, p_money));

	cache_get_value_name(0, "licenses", temp_string);
	sscanf(temp_string, "p<,>a<i>["#LICENSES_COUNT"]", player_info[playerid][p_licenses]);

	cache_get_value_name(0, "licenses_times", temp_string);
	sscanf(temp_string, "p<,>a<i>["#LICENSES_COUNT"]", player_info[playerid][p_licenses_times]);

	cache_get_value_name_int(0, "level", GetPlayerData(playerid, p_level));
	cache_get_value_name_int(0, "exp", GetPlayerData(playerid, p_exp));

	cache_get_value_name_float(0, "health", GetPlayerData(playerid, p_health));
	cache_get_value_name_int(0, "drunked_level", GetPlayerData(playerid, p_drunked_level));

	cache_get_value_name_int(0, "phone", GetPlayerData(playerid, p_phone));
	cache_get_value_name_int(0, "sim_card", GetPlayerData(playerid, p_sim_card));

	cache_get_value_name(0, "weapons", temp_string);
	sscanf(temp_string, "p<,>a<i>["#WEAPON_SLOTS_COUNT"]", player_info[playerid][p_weapons]);

	cache_get_value_name(0, "weapons_ammo", temp_string);
	sscanf(temp_string, "p<,>a<i>["#WEAPON_SLOTS_COUNT"]", player_info[playerid][p_weapons_ammo]);

	cache_get_value_name_int(0, "tenant_house_id", GetPlayerData(playerid, p_tenant_house_id));

	cache_delete(cache_id);
	OnPlayerAccountLoaded(playerid);

	ShowPlayerSpawnSelect(playerid);
	return true;
}

stock GiveMoney(playerid, money){
	if(GetPlayerData(playerid, p_money) + money >= 99999999) money = 99999999 - GetPlayerData(playerid, p_money);
	GivePlayerMoney(playerid, money);
	AddPlayerData(playerid, p_money, money);

	static const fmt_query[] = "UPDATE accounts SET money = %d WHERE id = %d LIMIT 1";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 24)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_money), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}

stock IsPlayerInRangeOfPlayer(playerid, targetplayerid, Float:range){
	if(!IsPlayerConnected(playerid) || !IsPlayerConnected(targetplayerid)) return false;

	if(GetPlayerVirtualWorld(playerid) != GetPlayerVirtualWorld(targetplayerid)
		|| GetPlayerInterior(playerid) != GetPlayerInterior(targetplayerid)) return false;

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	if(!IsPlayerInRangeOfPoint(targetplayerid, range, pos[0], pos[1], pos[2])) return false;
	
	return true;
}

stock GPSEnable(playerid, Float:x, Float:y, Float:z){
	if(GPSIsBlocked(playerid)) return false;

	DisablePlayerRaceCheckpoint(playerid);

	SetPlayerRaceCheckpoint(playerid, 1, x, y, z, x, y, z, 5.0);
	SetPVarInt(playerid, "GPS_Enabled", 1);
	return true;
}

stock GPSDisable(playerid){
	DisablePlayerRaceCheckpoint(playerid);
	DeletePVar(playerid, "GPS_Enabled");
	return true;
}

stock GPSIsBlocked(playerid){
	if(GetPlayerData(playerid, p_job) > 0 && GetPlayerData(playerid, p_job) != 6) return true;
	if(GetPlayerData(playerid, p_job) == 6 && GetPVarType(playerid, "JobProductsDelivery_OrderIndex")) return true;
	return false;
}

stock CheckPlayerLicenses(playerid){
	for(new i; i < LICENSES_COUNT; i++){
		if(player_info[playerid][p_licenses][i] && player_info[playerid][p_licenses_times][i] <= gettime() && player_info[playerid][p_licenses_times][i] > 0){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Срок действия лицензии \"%s\" истёк.",
				licensing_names[i]);
			small_string[0] = EOS;
			DeletePlayerLicense(playerid, i);
		}
	}
}

stock GivePlayerLicense(playerid, license_index, days){
	if(!player_logged[playerid]) return false;
	if(license_index < 0 || license_index >= LICENSES_COUNT) return false;
	if(days <= 0 || days > 1000) return false;

	player_info[playerid][p_licenses][license_index] = 1;
	player_info[playerid][p_licenses_times][license_index] = gettime() + (days * 86400);
	SavePlayerLicenses(playerid);
	return true;
}

stock DeletePlayerLicense(playerid, license_index){
	if(!player_logged[playerid]) return false;
	if(license_index < 0 || license_index >= LICENSES_COUNT) return false;

	player_info[playerid][p_licenses][license_index] = 0;
	player_info[playerid][p_licenses_times][license_index] = 0;
	SavePlayerLicenses(playerid);
	return true;
}

stock SavePlayerLicenses(playerid){
	if(!player_logged[playerid]) return false;

	new licenses_string[2][128];
	
	for(new i; i < LICENSES_COUNT; i++){
		format(licenses_string[0], 128, "%s%d%s",
			licenses_string[0], player_info[playerid][p_licenses][i],
			(i == LICENSES_COUNT - 1) ? ("") : (", "));

		format(licenses_string[1], 128, "%s%d%s",
			licenses_string[1], player_info[playerid][p_licenses_times][i],
			(i == LICENSES_COUNT - 1) ? ("") : (", "));
	}

	static const fmt_query[] = "UPDATE accounts SET licenses = '%s', licenses_times = '%s' WHERE id = %d LIMIT 1";
	new query[sizeof(fmt_query) + (-2 + 128) + (-2 + 128) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, licenses_string[0], licenses_string[1], GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}

stock SendMeAction(playerid, action[], is_automatic = false){
	new temp_string[144];
	format(temp_string, sizeof(temp_string),
		"%s%s %s",
		(is_automatic) ? ("* ") : (""),
		GetPlayerData(playerid, p_name), action);

	ProxDetector(playerid, 15.0, COLOR_ACTION, temp_string);
	return true;
}

stock SendDoAction(playerid, action[], is_automatic = false){
	new temp_string[144];
	format(temp_string, sizeof(temp_string),
		"%s%s (%s)",
		(is_automatic) ? ("* ") : (""),
		action, GetPlayerData(playerid, p_name));

	ProxDetector(playerid, 15.0, COLOR_ACTION, temp_string);
	return true;
}

stock ProxDetector(playerid, Float:max_range, color, const string[], Float:max_ratio = 1.6){
    new
        Float:pos_x,
        Float:pos_y,
        Float:pos_z,
        Float:range,
        Float:range_ratio,
        Float:range_with_ratio,
        clr_r, clr_g, clr_b,
        Float:color_r, Float:color_g, Float:color_b;

    if (!GetPlayerPos(playerid, pos_x, pos_y, pos_z)) {
        return 0;
    }

    color_r = float(color >> 24 & 0xFF);
    color_g = float(color >> 16 & 0xFF);
    color_b = float(color >> 8 & 0xFF);
    range_with_ratio = max_range * max_ratio;

	for (new i = GetPlayerPoolSize(); i != -1; i--) {
        if (!IsPlayerStreamedIn(playerid, i)) {
            continue;
        }

        range = GetPlayerDistanceFromPoint(i, pos_x, pos_y, pos_z);
        if (range > max_range) {
            continue;
        }

        range_ratio = (range_with_ratio - range) / range_with_ratio;
        clr_r = floatround(range_ratio * color_r);
        clr_g = floatround(range_ratio * color_g);
        clr_b = floatround(range_ratio * color_b);

        SendClientMessage(i, (color & 0xFF) | (clr_b << 8) | (clr_g << 16) | (clr_r << 24), string);
    }

    SendClientMessage(playerid, color, string);
    return 1;
}

stock GivePlayerExp(playerid, exp){
	AddPlayerData(playerid, p_exp, exp);
	if(GetPlayerData(playerid, p_level) < 100){
		if(GetPlayerData(playerid, p_exp) >= GetExpToNextLevel(GetPlayerData(playerid, p_level) + 1)){
			AddPlayerData(playerid, p_level, 1);
			SetPlayerScore(playerid, GetPlayerData(playerid, p_level));

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Поздравляем! Вы достигли %d уровня!",
				GetPlayerData(playerid, p_level));
			SendClientMessage(playerid, COLOR_GREEN, small_string);
			small_string[0] = EOS;

			OnPlayerLevelChanged(playerid, GetPlayerData(playerid, p_level) - 1, GetPlayerData(playerid, p_level));
		}
	}

	static const fmt_query[] = "UPDATE accounts SET level = %d, exp = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_level), GetPlayerData(playerid, p_exp), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}

stock GetExpToNextLevel(level){
    if(level < 1) return 0;
    return 4 * level + 4;
}

stock KickPlayer(playerid, time_ms = 50) { SetTimerEx("KickPlayerPublic", time_ms, false, "i", playerid); }

stock ShowMainMenu(playerid){
	Dialog_Open(playerid, Dialog:D_MAIN_MENU, DIALOG_STYLE_LIST,
		"Главное меню",
		""cl_blue"1. "cl_white"Статистика персонажа\n\
		"cl_blue"2. "cl_white"Помощь по серверу\n\
		"cl_blue"3. "cl_white"Связь с администрацией\n\
		"cl_blue"4. "cl_white"Настройки\n\
		"cl_blue"5. "cl_white"Донат-услуги",
		"Выбрать", "Закрыть");
	return true;
}

stock ShowHelpMenu(playerid){
	Dialog_Open(playerid, Dialog:D_HELP_MENU, DIALOG_STYLE_LIST,
		"Главное меню",
		""cl_blue"1. "cl_white"Общие команды\n\
		"cl_blue"2. "cl_white"Команды для транспорта\n\
		"cl_blue"3. "cl_white"Команды чата\n\
		"cl_blue"4. "cl_white"Команды управления жильём",
		"Выбрать", "Назад");
	return true;
}

stock ShowHelp(playerid, commands_type){
	if(commands_type < 0 || commands_type >= COMMAND_TYPE_MAX) return false;

	big_string[0] = EOS;
	for(new i; i < sizeof(command_info); i++){
		if(command_info[i][ci_type] != commands_type) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%s "cl_grey"- "cl_white"%s",
			big_string, command_info[i][ci_command], command_info[i][ci_description]);
	}

	Dialog_Open(playerid, Dialog:D_HELP, DIALOG_STYLE_MSGBOX,
		"Помощь по командам", big_string,
		"Назад", "");

	big_string[0] = EOS;
	return true;
}

stock GetPlayerId(const name[]){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!strcmp(name, GetPlayerData(i, p_name))) return i;
	}
	return INVALID_PLAYER_ID;
}

stock GetPlayerIdBySqlId(sql_id){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(GetPlayerData(i, p_id) != sql_id) continue;
		return i;
	}
	return INVALID_PLAYER_ID;
}

stock AddPlayerNotification(account_id, text[]){
	static const fmt_query[] = "INSERT INTO notifications (account_id, text) VALUES ('%d', '%s')";
	new query[sizeof(fmt_query) + (-2 + 256) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		account_id, text);
	mysql_tquery(mysql, query);
	return true;
}

stock CheckPlayerNotifications(playerid){
	static const fmt_query[] = "SELECT text FROM notifications WHERE account_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query, "CheckPlayerNotificationsPublic", "i", playerid);
	return true;
}

stock UpdatePlayerWeapons(playerid){
	for(new i; i < WEAPON_SLOTS_COUNT; i++){
		GetPlayerWeaponData(playerid, i, 
			player_info[playerid][p_weapons][i], 
			player_info[playerid][p_weapons_ammo][i]);
	}

	SavePlayerWeapons(playerid);
	return true;
}

stock SavePlayerWeapons(playerid){
	new weapon_str[128], weapon_ammo_str[128];

	for(new i; i < WEAPON_SLOTS_COUNT; i++){
		format(weapon_str, sizeof(weapon_str),
			"%s%d%s",
			weapon_str, player_info[playerid][p_weapons][i],
			(i == WEAPON_SLOTS_COUNT - 1) ? ("") : (", "));

		format(weapon_ammo_str, sizeof(weapon_ammo_str),
			"%s%d%s",
			weapon_ammo_str, player_info[playerid][p_weapons_ammo][i],
			(i == WEAPON_SLOTS_COUNT - 1) ? ("") : (", "));
	}

	static const fmt_query[] = "UPDATE accounts SET weapons = '%s', weapons_ammo = '%s' WHERE id = %d LIMIT 1";
	new query[sizeof(fmt_query) + ((-2 + 128) * 2) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		weapon_str, weapon_ammo_str, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}

stock GivePlayerAllWeapons(playerid){
	ResetPlayerWeapons(playerid);
	for(new i; i < WEAPON_SLOTS_COUNT; i++){
		GivePlayerWeapon(playerid, player_info[playerid][p_weapons][i], player_info[playerid][p_weapons_ammo][i]);
	}

	SetPlayerArmedWeapon(playerid, 0);
	return true;
}

stock GetPlayerColorEx(playerid){
	if(GetPVarType(playerid, "Mask")) return 0x7a766700;
	return 0xFFFFFFAA;
}

stock GetPlayerIdByPhoneNumber(number){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(GetPlayerData(i, p_sim_card) == -1) continue;
		if(GetPlayerData(i, p_sim_card) != number) continue;
		return i;
	}
	return INVALID_PLAYER_ID;
}

stock GetPlayerCallerId(playerid){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(GetPlayerData(i, p_sim_card) == -1) continue;
		if(!GetPVarType(i, "Phone_CallID")) continue;
		if(GetPVarInt(i, "Phone_CallID") != playerid) continue;
		return i;
	}
	return INVALID_PLAYER_ID;
}

stock ShowPlayerSpawnSelect(playerid){
	SelectTextDraw(playerid, -1);
	cef_emit_event(playerid,
		"showSpawnSelector");

	new is_house_blocked = true;

	/*for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;

		is_house_blocked = false;
		break;
	}*/

	if(GetPlayerData(playerid, p_tenant_house_id) != 0){
		is_house_blocked = false;
	}

	cef_emit_event(playerid, "setSpawnSelectorData",
		CEFINT(0),
		CEFINT(is_house_blocked),
		CEFINT(1),
		CEFINT(1));
	return true;
}