
DialogResponse:D_CAR_SHOWROOM_VEHICLE_INFO(playerid, response, listitem, inputtext[]){
	if(!response) return RemovePlayerFromVehicle(playerid);
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !GetPlayerVehicleID(playerid)) return RemovePlayerFromVehicle(playerid);
	if(GetPlayerData(playerid, p_money) < CarShowroom:GetVehiclePrice(GetPlayerVehicleID(playerid))){
		SendErrorMessage(playerid, "У Вас недостаточно средств для покупки данного транспорта.");
		RemovePlayerFromVehicle(playerid);
		return false;
	}

	static const fmt_query[] = "INSERT INTO vehicles (model_id, status, owner, price, color_1, color_2) VALUES (%d, %d, %d, %d, %d, %d)";
	new query[sizeof(fmt_query) + ((-2 + 11) * 6)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetVehicleModel(GetPlayerVehicleID(playerid)),
		VEHICLE_STATUS_PLAYER,
		GetPlayerData(playerid, p_id),
		CarShowroom:GetVehiclePrice(GetPlayerVehicleID(playerid)),
		1, 1);
	mysql_tquery(mysql, query, "BuyPlayerVehicle", "ii", playerid, GetPlayerVehicleID(playerid));
	return true;
}