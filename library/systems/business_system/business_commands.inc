
CMD:bpanel(playerid){
	if(!player_logged[playerid]) return false;
	
	Business:ShowPlayerSelectBusiness(playerid);
	return true;
}

CMD:fill(playerid){
	if(!player_logged[playerid]) return false;
	if(!IsPlayerInAnyVehicle(playerid) || !IsFueledVehicle(GetPlayerVehicleID(playerid))) return SendErrorMessage(playerid, "Вы должны находиться в транспорте");

	new business_index = -1;
	for(new i; i < BUSINESS_TYPES_COUNT; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_type) != BUSINESS_TYPE_GAS_STATION) continue;
		if(!IsPlayerInRangeOfPoint(playerid, 5.0, GetBusinessData(i, b_pos_x), GetBusinessData(i, b_pos_y), GetBusinessData(i, b_pos_z))) continue;

		business_index = i;
		break;
	}

	if(business_index == -1) return SendErrorMessage(playerid, "Вы должны находиться на АЗС!");

	Business:ShowMenu(playerid, business_index);
	return true;
}