
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

	player_logged[playerid] = true;
	SpawnPlayer(playerid);

	SetPlayerVirtualWorld(playerid, spawn_vw);
	SetPlayerInterior(playerid, spawn_int);
	SetCameraBehindPlayer(playerid);
	TogglePlayerControllable(playerid, true);
	return true;
}

stock LoadPlayer(playerid, Cache:cache_id){
	cache_set_active(cache_id);

	cache_get_value_name_int(0, "id", GetPlayerData(playerid, p_id));
	cache_get_value_name(0, "name", GetPlayerData(playerid, p_name));
	cache_get_value_name(0, "email", GetPlayerData(playerid, p_email));
	cache_get_value_name_int(0, "nationality", GetPlayerData(playerid, p_nationality));
	cache_get_value_name_int(0, "age", GetPlayerData(playerid, p_age));
	cache_get_value_name_int(0, "skin", GetPlayerData(playerid, p_skin));
	cache_get_value_name_int(0, "sex", GetPlayerData(playerid, p_sex));
	cache_get_value_name_int(0, "money", GetPlayerData(playerid, p_money));

	cache_delete(cache_id);
	OnPlayerAccountLoaded(playerid);

	SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
	return true;
}

stock GiveMoney(playerid, money){
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