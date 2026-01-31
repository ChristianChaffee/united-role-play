
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("spawnSelectorSelect", "spawnSelectorSelect");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	GetPlayerPos(playerid,
		GetPlayerData(playerid, p_spawn_x),
		GetPlayerData(playerid, p_spawn_y),
		GetPlayerData(playerid, p_spawn_z));

	GetPlayerFacingAngle(playerid,
		GetPlayerData(playerid, p_spawn_angle));

	SetPlayerData(playerid, p_spawn_vw, GetPlayerVirtualWorld(playerid));
	SetPlayerData(playerid, p_spawn_int, GetPlayerInterior(playerid));

	static const fmt_query[] = "UPDATE accounts SET spawn_x = %f, spawn_y = %f, spawn_z = %f, spawn_angle = %f, spawn_vw = %d, spawn_int = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 7)];

	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_spawn_x), GetPlayerData(playerid, p_spawn_y),
		GetPlayerData(playerid, p_spawn_z), GetPlayerData(playerid, p_spawn_angle),
		GetPlayerData(playerid, p_spawn_vw), GetPlayerData(playerid, p_spawn_int),
		GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerAccountLoaded(playerid){
	SpawnSelect:ShowUI(playerid);
	return continue(playerid);
}