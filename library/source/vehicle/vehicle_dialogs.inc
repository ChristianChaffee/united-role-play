
DialogResponse:D_VEHICLE_MENU(playerid, response, listitem, inputtext[]){
	if(response){
		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) > 0){
			switch(listitem){
				case 0:{
					if(!GetVehicleEngine(GetPlayerVehicleID(playerid)) && GetVehicleData(GetPlayerVehicleID(playerid), v_fuel) <= 0.0){
						SendErrorMessage(playerid, "В транспорте закончилось топливо.");
					}
					else{
						SetVehicleEngine(GetPlayerVehicleID(playerid), !GetVehicleEngine(GetPlayerVehicleID(playerid)));
					}
				}
				case 1: ChangePlayerVehicleDoors(playerid, GetPlayerVehicleID(playerid));
				case 2: SetVehicleLights(GetPlayerVehicleID(playerid), !GetVehicleLights(GetPlayerVehicleID(playerid)));
				case 3: SetVehicleBonnet(GetPlayerVehicleID(playerid), !GetVehicleBonnet(GetPlayerVehicleID(playerid)));
				case 4: SetVehicleBoot(GetPlayerVehicleID(playerid), !GetVehicleBoot(GetPlayerVehicleID(playerid)));
				default: ShowVehicleMenu(playerid);
			}
		}
	}
}

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
		mysql_tquery(mysql, query, "LoadPlayerVehicle", "i", playerid);
	}
	return true;
}