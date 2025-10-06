
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM enter_exits", "LoadEnterExits");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		for(new i; i < TOTAL_ENTER_EXITS; i++){
			if(IsPlayerInRangeOfPoint(playerid, 1.0, GetEnterExitData(i, ee_enter_pos_x), GetEnterExitData(i, ee_enter_pos_y), GetEnterExitData(i, ee_enter_pos_z))
				&& GetPlayerVirtualWorld(playerid) == GetEnterExitData(i, ee_enter_vw)
				&& GetPlayerInterior(playerid) == GetEnterExitData(i, ee_enter_int)){

				EnterExits:EnterPlayer(playerid, i);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			if(IsPlayerInRangeOfPoint(playerid, 1.0, GetEnterExitData(i, ee_exit_pos_x), GetEnterExitData(i, ee_exit_pos_y), GetEnterExitData(i, ee_exit_pos_z))
				&& GetPlayerVirtualWorld(playerid) == GetEnterExitData(i, ee_exit_vw)
				&& GetPlayerInterior(playerid) == GetEnterExitData(i, ee_exit_int)){

				EnterExits:ExitPlayer(playerid, i);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}