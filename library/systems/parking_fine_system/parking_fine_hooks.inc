
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	CreateDynamic3DTextLabel("Штраф-стоянка\n"cl_grey"'Left ALT'", COLOR_LIME, 
		PARKING_FINE_POS, 20.0, _, _, 1, 0, 0);

	CreateDynamicPickup(PARKING_FINE_PICKUP_MODEL_ID, 23, 
		PARKING_FINE_POS, 0, 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK && IsPlayerInRangeOfPoint(playerid, 1.5, PARKING_FINE_POS)){
		static const fmt_query[] = "SELECT id, model_id FROM vehicles WHERE owner = %d AND status = %d AND pos_x = 0 AND pos_y = 0 AND pos_z = 0";
		new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerData(playerid, p_id), VEHICLE_STATUS_PLAYER);
		mysql_tquery(mysql, query, "CheckPlayerParkFineVehicles", "i", playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}