
stock GetVehicleDriver(vehicleid){
	if(!IsValidVehicle(vehicleid)) return INVALID_PLAYER_ID;

	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(GetPlayerState(i) != PLAYER_STATE_DRIVER) continue;
		if(GetPlayerVehicleID(i) != vehicleid) continue;

		return i;
	}
	return INVALID_PLAYER_ID;
}

stock SetVehicleEngine(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	engine = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return status;
}

stock GetVehicleEngine(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return engine;
}

stock SetVehicleDoors(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	doors = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return status;
}

stock SetVehicleLights(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	lights = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return status;
}

stock GetVehicleSpeed(vehicleid){
	new Float:velocity[3];
	GetVehicleVelocity(vehicleid, velocity[0], velocity[1], velocity[2]);
	return floatround(floatsqroot(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]) * 180.00);
}

stock Float:GetVehicleHealthEx(vehicleid){
	if(!IsValidVehicle(vehicleid)) return 0.0;

	GetVehicleHealth(vehicleid, GetVehicleData(vehicleid, v_health));
	return GetVehicleData(vehicleid, v_health);
}