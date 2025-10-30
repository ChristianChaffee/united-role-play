
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	ManualVehicleEngineAndLights();
	SetTimer("VehicleFuelTimer", VEHICLE_FUEL_TIME, true);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook function CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren=0){
	new vehicleid = continue(vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren);

	SetVehicleData(vehicleid, v_model_id, vehicletype);

	SetVehicleData(vehicleid, v_pos_spawn_x, x);
	SetVehicleData(vehicleid, v_pos_spawn_y, y);
	SetVehicleData(vehicleid, v_pos_spawn_z, z);
	SetVehicleData(vehicleid, v_pos_spawn_angle, rotation);

	SetVehicleData(vehicleid, v_color_1, color1);
	SetVehicleData(vehicleid, v_color_2, color2);

	SetVehicleData(vehicleid, v_siren, addsiren);
	SetVehicleData(vehicleid, v_respawn_delay, respawn_delay);
	SetVehicleFuel(vehicleid, 50.0);

	SetVehicleEngine(vehicleid, false);
	SetVehicleDoors(vehicleid, false);
	SetVehicleLights(vehicleid, false);
	SetVehicleBonnet(vehicleid, false);
	SetVehicleBoot(vehicleid, false);
	return vehicleid;
}

hook function SetVehicleVirtualWorld(vehicleid, worldid){
	if(!IsValidVehicle(vehicleid)) return false;

	SetVehicleData(vehicleid, v_vw, worldid);
	return continue(vehicleid, worldid);
}

hook function LinkVehicleToInterior(vehicleid, interiorid){
	if(!IsValidVehicle(vehicleid)) return false;

	SetVehicleData(vehicleid, v_int, interiorid);
	return continue(vehicleid, interiorid);
}

hook function DestroyVehicle(vehicleid){
	new result = continue(vehicleid);

	if(result){
		vehicle_info[vehicleid] = vehicle_info_empty;
	}
	return result;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_SUBMISSION && GetPlayerState(playerid) == PLAYER_STATE_DRIVER){
		if(!GetVehicleEngine(GetPlayerVehicleID(playerid)) && GetVehicleData(GetPlayerVehicleID(playerid), v_fuel) <= 0.0){
			SendErrorMessage(playerid, "В транспорте закончилось топливо.");
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
		SetVehicleEngine(GetPlayerVehicleID(playerid), !GetVehicleEngine(GetPlayerVehicleID(playerid)));
	}
	if(newkeys == KEY_ACTION && GetPlayerState(playerid) == PLAYER_STATE_DRIVER){
		ShowVehicleMenu(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}