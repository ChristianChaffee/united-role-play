
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	for(new i; i < MAX_PLAYERS; i++) { PlayerNeeds:ClearValues(i); }
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid){
	PlayerNeeds:ClearValues(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	PlayerNeeds:ClearValues(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock LoadPlayer(playerid, Cache:cache_id){
	cache_set_active(cache_id);

	new temp_string[128];
	cache_get_value_name(0, "needs", temp_string);
	sscanf(temp_string, "p<,>a<i>["#PLAYER_NEEDS_COUNT"]", player_need_info[playerid]);

	return continue(playerid, _:cache_id);
}

hook stock OnPlayerMinuteUpdate(playerid){
	if(player_logged[playerid]){
		for(new i; i < PLAYER_NEEDS_COUNT; i++){
			
			AddPlayerNeedValue(playerid, i, -GetNeedData(i, n_minus_per_minute));
			if(GetPlayerNeedValue(playerid, i) < 0) SetPlayerNeedValue(playerid, i, 0);

			if(GetPlayerNeedValue(playerid, i) <= GetNeedData(i, n_critical_value)){
				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"[Потребности]: "cl_white"Показатель "cl_lime"%s "cl_white"достиг критического значения в "cl_lime"%d единиц(-у)"cl_white".",
					GetNeedData(i, n_message_name), GetPlayerNeedValue(playerid, i));
				SendClientMessage(playerid, COLOR_LIME, small_string);

				format(small_string, sizeof(small_string),
					"[Подсказка]: "cl_white"%s.",
					GetNeedData(i, n_advice));
				SendClientMessage(playerid, COLOR_LIME, small_string);
				small_string[0] = EOS;

				SetPlayerHealth(playerid, GetPlayerData(playerid, p_health) - 1.0);
			}
		}
		PlayerNeeds:SaveNeeds(playerid);
	}
	return continue(playerid);
}