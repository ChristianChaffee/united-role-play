
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		for(new i; i < MAX_BUSINESS; i++){
			if(!GetBusinessData(i, b_id)) continue;
		
			if(IsPlayerInRangeOfPoint(playerid, 2.0, GetBusinessData(i, b_pos_x), GetBusinessData(i, b_pos_y), GetBusinessData(i, b_pos_z))){

				SetPlayerPos(playerid,
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_x),
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_y),
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_z));
				SetPlayerFacingAngle(playerid,
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_angle));
				SetPlayerInterior(playerid,
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_int));
				SetPlayerVirtualWorld(playerid,
					GetBusinessData(i, b_id));
				break;
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnMySQLConnect(){
	mysql_tquery(mysql, "SELECT * FROM business", "LoadBusinesses");
	return continue();
}