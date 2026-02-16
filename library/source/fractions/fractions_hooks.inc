
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM fractions", "LoadFractions", "i", -1);
	mysql_tquery(mysql, "SELECT * FROM fraction_skins", "LoadFractionSkins", "i", -1);
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