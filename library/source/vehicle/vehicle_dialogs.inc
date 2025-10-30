
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