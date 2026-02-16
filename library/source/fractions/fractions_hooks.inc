
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM fractions", "LoadFractions", "i", -1);
	mysql_tquery(mysql, "SELECT * FROM fraction_skins", "LoadFractionSkins", "i", -1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	static const fmt_query[] = "UPDATE accounts SET fraction_gun_cooldown = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_fraction_gun_cooldown), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK && !IsPlayerInAnyVehicle(playerid)){
		for(new fraction_index; fraction_index < MAX_FRACTIONS; fraction_index++){
			if(!GetFractionData(fraction_index, f_id)) continue;
			if(GetFractionData(fraction_index, f_dressroom_x) == 0.0 && GetFractionData(fraction_index, f_dressroom_y) == 0.0
				&& GetFractionData(fraction_index, f_dressroom_z) == 0.0) continue;

			if(IsPlayerInRangeOfPoint(playerid, 2.0, 
				GetFractionData(fraction_index, f_dressroom_x), GetFractionData(fraction_index, f_dressroom_y),
				GetFractionData(fraction_index, f_dressroom_z))
				&& GetPlayerVirtualWorld(playerid) == GetFractionData(fraction_index, f_dressroom_vw)
				&& GetPlayerInterior(playerid) == GetFractionData(fraction_index, f_dressroom_int)){

				if(GetPlayerData(playerid, p_fraction) != GetFractionData(fraction_index, f_id)){
					small_string[0] = EOS;
					format(small_string, sizeof(small_string),
						"Данная раздевалка принадлежит фракции \"%s\".",
						GetFractionData(fraction_index, f_name));
					SendErrorMessage(playerid, small_string);
					small_string[0] = EOS;
				}
				else{
					if(GetPlayerData(playerid, p_fraction_duty)){
						SendClientMessage(playerid, COLOR_BLUE, "Вы завершили рабочий день.");
						SetPlayerData(playerid, p_fraction_duty, false);
						SetPlayerSkin(playerid, GetPlayerData(playerid, p_skin));
						SetPlayerColor(playerid, GetPlayerColorEx(playerid));
					}
					else{
						SendClientMessage(playerid, COLOR_BLUE, "Вы начали рабочий день.");
						SetPlayerData(playerid, p_fraction_duty, true);
					
						if(Fraction:GetPlayerSkinIndex(playerid) != -1){
							SetPlayerSkin(playerid, GetFractionSkinData(Fraction:GetPlayerSkinIndex(playerid), fs_skin_id));
						}

						SetPlayerColor(playerid, GetPlayerColorEx(playerid));
					}
				}
			}
			else if(IsPlayerInRangeOfPoint(playerid, 2.0, 
				GetFractionData(fraction_index, f_gun_x), GetFractionData(fraction_index, f_gun_y),
				GetFractionData(fraction_index, f_gun_z))
				&& GetPlayerVirtualWorld(playerid) == GetFractionData(fraction_index, f_gun_vw)
				&& GetPlayerInterior(playerid) == GetFractionData(fraction_index, f_gun_int)){

				if(GetPlayerData(playerid, p_fraction) != GetFractionData(fraction_index, f_id)){
					small_string[0] = EOS;
					format(small_string, sizeof(small_string),
						"Данная оружейная фракции \"%s\".",
						GetFractionData(fraction_index, f_name));
					SendErrorMessage(playerid, small_string);
					small_string[0] = EOS;
				}
				else{
					if(!GetPlayerData(playerid, p_fraction_duty)){
						SendErrorMessage(playerid, "Вам необходимо начать рабочий день.");
					}
					else{
						Fraction:GiveFracWeapon(playerid);
					}
				}
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerCommandReceived(playerid, cmd[], params[], flags){
	for(new i; i < FRACTION_COMMANDS_COUNT; i++){
		if(!strcmp(cmd, fraction_commands_info[i][fc_command])){

			new fraction_index = GetPlayerFractionIndex(playerid);
			if(fraction_index == -1){
				SendErrorMessage(playerid, "У Вас нет доступа к этой команде.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			new is_find;
			for(new g; g < FRACTIONS_TYPES_COUNT; g++){
				if(fraction_commands_info[i][fc_types][g] == GetFractionData(fraction_index, f_type)){
					is_find = true;
					break;
				}
			}

			if(!is_find){
				SendErrorMessage(playerid, "У Вас нет доступа к этой команде.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(GetPlayerData(playerid, p_fraction_rank) < fraction_commands_info[i][fc_access_rank]){
				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"Данная команда доступна с %d-го ранга.", 
					fraction_commands_info[i][fc_access_rank]);
				SendErrorMessage(playerid, small_string);
				small_string[0] = EOS;
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(!GetPlayerData(playerid, p_fraction_duty) && fraction_commands_info[i][fc_on_duty_check]){
				SendErrorMessage(playerid, "Вы должны начать рабочий день.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerData(playerid, p_fraction_gun_cooldown) > 0){
		SetPlayerData(playerid, p_fraction_gun_cooldown, GetPlayerData(playerid, p_fraction_gun_cooldown) - 1);
		if(GetPlayerData(playerid, p_fraction_gun_cooldown) <= 0){
			static const fmt_query[] = "UPDATE accounts SET fraction_gun_cooldown = 0 WHERE id = %d";
			new query[sizeof(fmt_query) + (-2 + 11)];
			mysql_format(mysql, query, sizeof(query), fmt_query,
				GetPlayerData(playerid, p_id));
			mysql_tquery(mysql, query);
		}
	}
	return continue(playerid);
}