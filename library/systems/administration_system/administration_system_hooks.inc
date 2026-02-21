
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	for(new i; i < MAX_PLAYERS; i++) { player_admin_info[i] = player_admin_info_default; }
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	if(GetPVarType(playerid, "Admin_AddFracVehVehicleid") && IsValidVehicle(GetPVarInt(playerid, "Admin_AddFracVehVehicleid"))){
		DestroyVehicle(GetPVarInt(playerid, "Admin_AddFracVehVehicleid"));
	}

	player_admin_info[playerid] = player_admin_info_default;
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerCommandReceived(playerid, cmd[], params[], flags){
	for(new i; i < sizeof(admin_command_info); i++){
		if(!strcmp(cmd, admin_command_info[i][ac_command])){

			if(!GetPlayerAdminData(playerid, pa_id)) return Y_HOOKS_CONTINUE_RETURN_0;
			if(GetPlayerAdminData(playerid, pa_level) < admin_command_info[i][ac_level]) return Y_HOOKS_CONTINUE_RETURN_0;
			if(!GetPlayerAdminData(playerid, pa_logged)){
				callcmd::alogin(playerid);
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			

			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate){
	if(oldstate == PLAYER_STATE_DRIVER && GetPVarType(playerid, "Admin_AddFracVehVehicleid") && IsValidVehicle(GetPVarInt(playerid, "Admin_AddFracVehVehicleid"))){
		DestroyVehicle(GetPVarInt(playerid, "Admin_AddFracVehVehicleid"));
		SendClientMessage(playerid, COLOR_LIGHT_RED, "Создание фракционного транспорта отменено.");
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerAccountLoaded(playerid){
	static const fmt_query[] = "SELECT * FROM administration WHERE account_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query, "LoadPlayerAdminAccount", "i", playerid);
	return continue(playerid);
}