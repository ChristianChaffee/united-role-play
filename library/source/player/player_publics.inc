
forward CheckPlayerNotificationsPublic(playerid);
public CheckPlayerNotificationsPublic(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return false;

	for(new i, temp_text[256]; i < rows; i++){
		cache_get_value_name(i, "text", temp_text);

		SendClientMessage(playerid, -1, temp_text);
	}

	static const fmt_query[] = "DELETE FROM notifications WHERE account_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}

forward spawnSelectorSelect(playerid, const arguments[]);
public spawnSelectorSelect(playerid, const arguments[]){
	new selected_spawn_index;
	sscanf(arguments, "i", selected_spawn_index);

	switch(selected_spawn_index){
		case 0: SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
		case 1: SetPlayerSpawn(playerid, SPAWN_TYPE_HOUSE);
		case 2: SetPlayerSpawn(playerid, SPAWN_TYPE_EXIT);
		case 3: SetPlayerSpawn(playerid, SPAWN_TYPE_FRACTION);
	}

	cef_emit_event(playerid, "hideSpawnSelector");
	CancelSelectTextDraw(playerid);
	return true;
}