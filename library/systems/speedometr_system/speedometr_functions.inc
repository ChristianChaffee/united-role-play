
stock Speedometr:ShowPlayerSpeedometr(playerid, status){
	if(!status){
		DeletePVar(playerid, "Speedometr_IsShowed");

		for(new i; i < sizeof(TDEditor_TD); i++){
			TextDrawHideForPlayer(playerid, TDEditor_TD[i]);
		}
		for(new i; i < 11; i++){
			PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][i]);
		}
	}
	else{
		if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !GetPlayerVehicleID(playerid)) return false;

		SetPVarInt(playerid, "Speedometr_IsShowed", 1);
		for(new i; i < sizeof(TDEditor_TD); i++){
			TextDrawShowForPlayer(playerid, TDEditor_TD[i]);
		}
		for(new i; i < 11; i++){
			PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][i]);
		}
		Speedometr:UpdateAll(playerid);
	}
	return status;
}

stock Speedometr:UpdateAll(playerid){
	if(!GetPVarType(playerid, "Speedometr_IsShowed")) return false;

	new vehicleid = GetPlayerVehicleID(playerid);
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !vehicleid) return false;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	//------- Engine
	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][1]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][1], (engine) ? (SPEEDOMETR_COLOR_ON) : (SPEEDOMETR_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][1]);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][7]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][7], (engine) ? (SPEEDOMETR_TEXT_COLOR_ON) : (SPEEDOMETR_TEXT_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][7]);

	//------- Doors
	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][2]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][2], (!doors) ? (SPEEDOMETR_COLOR_ON) : (SPEEDOMETR_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][2]);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][8]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][8], (!doors) ? (SPEEDOMETR_TEXT_COLOR_ON) : (SPEEDOMETR_TEXT_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][8]);

	//------- Lights
	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][3]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][3], (lights) ? (SPEEDOMETR_COLOR_ON) : (SPEEDOMETR_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][3]);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][9]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][9], (lights) ? (SPEEDOMETR_TEXT_COLOR_ON) : (SPEEDOMETR_TEXT_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][9]);

	//------- Speed
	Speedometr:UpdateSpeed(playerid);
	return true;
}

stock Speedometr:UpdateSpeed(playerid){
	if(!GetPVarType(playerid, "Speedometr_IsShowed")) return false;

	new vehicleid = GetPlayerVehicleID(playerid);
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !vehicleid) return false;

	speed_string[0] = EOS;
	format(speed_string, sizeof(speed_string), "%d", GetVehicleSpeed(vehicleid));
	PlayerTextDrawSetString(playerid, TDEditor_PTD[playerid][6], speed_string);
	speed_string[0] = EOS;

	health_string[0] = EOS;
	format(health_string, sizeof(health_string), "HEALTH:_%d", floatround(Float:GetVehicleHealthEx(vehicleid)));
	PlayerTextDrawSetString(playerid, TDEditor_PTD[playerid][10], health_string);
	health_string[0] = EOS;
	return true;
}