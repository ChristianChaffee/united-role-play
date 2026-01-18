
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM garages", "LoadGarages", "i", -1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && newkeys == KEY_WALK){
		if(GetPlayerVirtualWorld(playerid) == 0){
			for(new i; i < MAX_GARAGES; i++){
				if(!GetGarageData(i, g_id)) continue;
				if(!IsPlayerInRangeOfPoint(playerid, 3.0, GetGarageData(i, g_pos_x), GetGarageData(i, g_pos_y), GetGarageData(i, g_pos_z))) continue;
				if(!House:IsPlayerHaveAccessToGarage(playerid, i)) continue;

				House:GarageEnter(playerid, i);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}
		else if(GetPlayerInterior(playerid) == GARAGE_TP_INT){
			for(new i; i < MAX_GARAGES; i++){
				if(!GetGarageData(i, g_id)) continue;
				if(GetPlayerVirtualWorld(playerid) != GetGarageData(i, g_id)) continue;
				if(!IsPlayerInRangeOfPoint(playerid, 3.0, GARAGE_TP_POS)) continue;

				House:GarageExit(playerid, i);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}
	}
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER && newkeys == KEY_CROUCH){
		if(GetPlayerVirtualWorld(playerid) == 0){
			for(new i; i < MAX_GARAGES; i++){
				if(!GetGarageData(i, g_id)) continue;
				if(!IsPlayerInRangeOfPoint(playerid, 5.0, GetGarageData(i, g_pos_x), GetGarageData(i, g_pos_y), GetGarageData(i, g_pos_z))) continue;
				if(!House:IsPlayerHaveAccessToGarage(playerid, i)) continue;

				House:GarageEnter(playerid, i);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}
		else if(GetPlayerInterior(playerid) == GARAGE_TP_INT){
			for(new i; i < MAX_GARAGES; i++){
				if(!GetGarageData(i, g_id)) continue;
				if(GetPlayerVirtualWorld(playerid) != GetGarageData(i, g_id)) continue;
				if(!IsPlayerInRangeOfPoint(playerid, 5.0, GARAGE_TP_POS)) continue;

				House:GarageExit(playerid, i);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}