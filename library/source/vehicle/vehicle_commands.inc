
CMD:cars(playerid){
	if(!player_logged[playerid]) return false;

	static const fmt_query[] = "SELECT id, model_id FROM vehicles WHERE owner = %d AND status = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id), VEHICLE_STATUS_PLAYER);
	mysql_tquery(mysql, query, "ShowPlayerCars", "i", playerid);
	return true;
}

CMD:park(playerid){
	if(!player_logged[playerid]) return false;
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !GetPlayerVehicleID(playerid)
		|| GetVehicleData(GetPlayerVehicleID(playerid), v_owner) != GetPlayerData(playerid, p_id)
		|| GetVehicleData(GetPlayerVehicleID(playerid), v_status) != VEHICLE_STATUS_PLAYER){

		return SendErrorMessage(playerid, "¬ы должны находитьс€ за рулем личного транспорта.");
	}

	new Float:pos[4];
	GetVehiclePos(GetPlayerVehicleID(playerid), pos[0], pos[1], pos[2]);
	GetVehicleZAngle(GetPlayerVehicleID(playerid), pos[3]);

	static const fmt_query[] = "UPDATE vehicles SET pos_x = %f, pos_y = %f, pos_z = %f, pos_angle = %f, pos_vw = %d, pos_int = %d WHERE id = %d LIMIT 1";
	new query[sizeof(fmt_query) + ((-2 + 11) * 7)];
	mysql_format(mysql, query, sizeof(query), fmt_query, 
		pos[0], pos[1], pos[2], pos[3], GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid),
		GetVehicleData(GetPlayerVehicleID(playerid), v_id));
	mysql_tquery(mysql, query);

	SendClientMessage(playerid, COLOR_LIME, "¬ы успешно припарковали транспорт.");
	return true;
}

CMD:lock(playerid){
	if(!player_logged[playerid]) return false;
	new vehicleid = GetNearestVehicleByRadius(playerid, 10.0);
	if(vehicleid == INVALID_VEHICLE_ID
		|| GetVehicleData(vehicleid, v_owner) != GetPlayerData(playerid, p_id)
		|| GetVehicleData(vehicleid, v_status) != VEHICLE_STATUS_PLAYER){

		return SendErrorMessage(playerid, "¬ы должны находитьс€ около личного транспорта.");
	}

	ChangePlayerVehicleDoors(playerid, vehicleid);
	return true;
}