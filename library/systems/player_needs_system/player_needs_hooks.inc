
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

	new temp_string[128], sscanf_string[32];
	format(sscanf_string, sizeof(sscanf_string), "p<,>a<i>[%d]", PLAYER_NEEDS_COUNT);

	cache_get_value_name(0, "needs", temp_string);
	sscanf(temp_string, sscanf_string, player_need_info[playerid]);

	return continue(playerid, _:cache_id);
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPVarType(playerid, "PlayerNeeds_PissingTime") && GetPVarInt(playerid, "PlayerNeeds_PissingTime") > 0){
		SetPVarInt(playerid, "PlayerNeeds_PissingTime", GetPVarInt(playerid, "PlayerNeeds_PissingTime") - 1);
		if(GetPVarInt(playerid, "PlayerNeeds_PissingTime") <= 0){
			DeletePVar(playerid, "PlayerNeeds_PissingTime");
			TogglePlayerControllable(playerid, true);

			SendClientMessage(playerid, COLOR_LIME, "Вы справили нужду!");
			SetPlayerNeedValue(playerid, PLAYER_NEED_URINE, 100);
			PlayerNeeds:SaveNeeds(playerid);

			ClearAnimations(playerid);
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		}
	}
	return continue(playerid);
}

hook stock OnPlayerMinuteUpdate(playerid){
	if(player_logged[playerid] && (GetPlayerAdminData(playerid, pa_id) || !GetPlayerAdminData(playerid, pa_logged))){
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