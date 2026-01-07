
stock Speedometr:ShowPlayerSpeedometr(playerid, status){
	if(!status){
		DeletePVar(playerid, "Speedometr_IsShowed");

		cef_emit_event(playerid, "hideSpeedometer");
	}
	else{
		if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !GetPlayerVehicleID(playerid)) return false;

		SetPVarInt(playerid, "Speedometr_IsShowed", 1);
		cef_emit_event(playerid, "showSpeedometer");

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

	cef_emit_event(playerid, "updateDoorStatus", CEFINT((!doors) ? (1) : (0)));
	cef_emit_event(playerid, "updateEngineStatus", CEFINT((engine) ? (1) : (0)));
	cef_emit_event(playerid, "updateHeadlightsStatus", CEFINT((lights) ? (1) : (0)));

	//------- Speed
	Speedometr:UpdateSpeed(playerid);

	//------- Fuel
	Speedometr:UpdateFuel(playerid);
	return true;
}

stock Speedometr:UpdateSpeed(playerid){
	if(!GetPVarType(playerid, "Speedometr_IsShowed")) return false;

	new vehicleid = GetPlayerVehicleID(playerid);
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !vehicleid) return false;

	cef_emit_event(playerid, "updateSpeed", CEFINT(GetVehicleSpeed(vehicleid)));
	return true;
}

stock Speedometr:UpdateFuel(playerid){
	if(!GetPVarType(playerid, "Speedometr_IsShowed")) return false;

	new vehicleid = GetPlayerVehicleID(playerid);
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !vehicleid) return false;

	cef_emit_event(playerid, "updateSpeed", CEFINT(floatround(GetVehicleData(vehicleid, v_fuel))));
	return true;
}