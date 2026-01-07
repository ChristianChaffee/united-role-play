
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook stock SetVehicleEngine(vehicleid, status){
	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return continue(vehicleid, status);

	cef_emit_event(playerid, "updateEngineStatus", CEFINT((status) ? (1) : (0)));
	return continue(vehicleid, status);
}

hook stock SetVehicleDoors(vehicleid, status){
	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return continue(vehicleid, status);

	cef_emit_event(playerid, "updateDoorStatus", CEFINT((!status) ? (1) : (0)));
	return continue(vehicleid, status);
}

hook stock SetVehicleLights(vehicleid, status){
	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return continue(vehicleid, status);

	cef_emit_event(playerid, "updateHeadlightsStatus", CEFINT((status) ? (1) : (0)));
	return continue(vehicleid, status);
}

hook stock SetVehicleFuel(vehicleid, Float:fuel){
	new result = continue(vehicleid, fuel);

	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return result;

	Speedometr:UpdateFuel(playerid);
	return result;
}

hook OnPlayerStateChange(playerid, newstate, oldstate){
	if(newstate == PLAYER_STATE_DRIVER){
		Speedometr:ShowPlayerSpeedometr(playerid, true);
	}
	if(oldstate == PLAYER_STATE_DRIVER){
		Speedometr:ShowPlayerSpeedometr(playerid, false);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit(){
	SetTimer("SpeedometrTimer", SPEEDOMETR_TIMER, true);
	return Y_HOOKS_CONTINUE_RETURN_1;
}