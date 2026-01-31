
DialogResponse:D_PLAYER_CARS_LIST(playerid, response, listitem, inputtext[]){
	if(response){
		if(FindVehicleById(player_listitems[playerid][listitem]) != INVALID_VEHICLE_ID){
			SendClientMessage(playerid, COLOR_LIGHT_RED, "Вы выгрузили транспорт с сервера.");
			DestroyVehicle(FindVehicleById(player_listitems[playerid][listitem]));
			return true;
		}

		static const fmt_query[] = "SELECT * FROM vehicles WHERE id = %d AND owner = %d AND status = %d LIMIT 1";
		new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			player_listitems[playerid][listitem], GetPlayerData(playerid, p_id), VEHICLE_STATUS_PLAYER);
		mysql_tquery(mysql, query, "CheckPlayerVehicle", "i", playerid);
	}
	return true;
}