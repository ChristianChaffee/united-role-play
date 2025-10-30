
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	ManualVehicleEngineAndLights();
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

	SetVehicleEngine(vehicleid, false);
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