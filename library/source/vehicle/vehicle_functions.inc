
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

//-----------------------------------------------
stock SetVehicleEngine(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	engine = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	SetVehicleLights(vehicleid, status);
	return status;
}

stock GetVehicleEngine(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return engine;
}

//-----------------------------------------------
stock SetVehicleDoors(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	doors = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return status;
}

stock GetVehicleDoors(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return doors;
}

//-----------------------------------------------
stock SetVehicleLights(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	lights = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return status;
}

stock GetVehicleLights(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return lights;
}

//-----------------------------------------------
stock SetVehicleBonnet(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	bonnet = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return status;
}

stock GetVehicleBonnet(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return bonnet;
}

//-----------------------------------------------
stock SetVehicleBoot(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	boot = status;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return status;
}

stock GetVehicleBoot(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	return boot;
}

//-----------------------------------------------
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

stock SetVehicleFuel(vehicleid, Float:fuel){
	if(!IsValidVehicle(vehicleid)) return false;

	SetVehicleData(vehicleid, v_fuel, fuel);
	return true;
}

stock IsPlayerInRangeOfVehicle(playerid, vehicleid, Float:radius){
	if(!IsValidVehicle(vehicleid)) return false;

	if(GetPlayerVirtualWorld(playerid) != GetVehicleData(vehicleid, v_vw)
		|| GetPlayerInterior(playerid) != GetVehicleData(vehicleid, v_int)) return false;

	new Float:vehicle_pos[3];
	GetVehiclePos(vehicleid, vehicle_pos[0], vehicle_pos[1], vehicle_pos[2]);

	if(!IsPlayerInRangeOfPoint(playerid, radius, vehicle_pos[0], vehicle_pos[1], vehicle_pos[2])) return false;
	return true;
}

stock ChangePlayerVehicleDoors(playerid, vehicleid){
	if(!IsValidVehicle(vehicleid)) return false;
	if(!IsPlayerInRangeOfVehicle(playerid, vehicleid, 10.0)){
		SendErrorMessage(playerid, "Рядом с Вами нет подходящего транспорта.");
		return false;
	}

	SetVehicleDoors(vehicleid, !GetVehicleDoors(vehicleid));
	GameTextForPlayer(playerid, (!GetVehicleDoors(vehicleid)) ? ("~g~UNLOCK") : ("~r~LOCK"), 3000, 3);
	return true;
}

stock SetVehicleStatus(vehicleid, status){
	if(!IsValidVehicle(vehicleid)) return false;
	if(status < 0 || status >= VEHICLE_STATUS_MAX) return false;

	SetVehicleData(vehicleid, v_status, status);
	if(status == VEHICLE_STATUS_PLAYER) SetVehicleDoors(vehicleid, 1);
	return true;
}

stock GetVehicleParamIndex(vehicleid){
	if(!IsValidVehicle(vehicleid)) return 0;
	for(new i; i < VEHICLE_MODELS_COUNT; i++){
		if(GetVehicleParamsData(i, vp_model_id) == GetVehicleModel(vehicleid)) return i;
	}
	return 0;
}

stock GetVehicleParamIndexByModel(modelid){
	for(new i; i < VEHICLE_MODELS_COUNT; i++){
		if(GetVehicleParamsData(i, vp_model_id) == modelid) return i;
	}
	return 0;
}

stock FindVehicleById(id){
	for(new vehicleid, j = GetVehiclePoolSize(); vehicleid <= j; vehicleid++){
		if(IsValidVehicle(vehicleid) && GetVehicleData(vehicleid, v_id) == id) return vehicleid;
	}
	return INVALID_VEHICLE_ID;
}

stock Text3D:CreateVehicle3DText(vehicleid, const text[], color, Float:drawdistance){
	if(!IsValidVehicle(vehicleid)) return Text3D:-1;
	
	GetVehicleData(vehicleid, v_text) = CreateDynamic3DTextLabel(text, color, 0.0, 0.0, 0.0, drawdistance, _, vehicleid);
	return GetVehicleData(vehicleid, v_text);
}

stock DestroyVehicleText3D(vehicleid){
	if(!IsValidVehicle(vehicleid)) return false;
	if(GetVehicleData(vehicleid, v_text) == Text3D:-1) return false;

	DestroyDynamic3DTextLabel(GetVehicleData(vehicleid, v_text));
	GetVehicleData(vehicleid, v_text) = Text3D:-1;
	return true;
}

stock GetNearestVehicleByRadius(playerid, Float:radius){

	new 
		Float:pos[3], 
		Float:distance = radius * 10.0,
		return_vehicleid = INVALID_VEHICLE_ID;

	for(new vehicleid, j = GetVehiclePoolSize(); vehicleid <= j; vehicleid++){
		GetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);
		if(!IsPlayerInRangeOfPoint(playerid, radius, pos[0], pos[1], pos[2])) continue;
		if(distance < GetPlayerDistanceFromPoint(playerid, pos[0], pos[1], pos[2])) continue;

		distance = GetPlayerDistanceFromPoint(playerid, pos[0], pos[1], pos[2]);
		return_vehicleid = vehicleid;
	}
	return return_vehicleid;
}

stock CreatePlayerVehicle(playerid, id, Float:pos_x = 0.0, Float:pos_y = 0.0, Float:pos_z = 0.0, Float:pos_angle = 0.0, vw = 0, int = 0){
	static const fmt_query[] = "SELECT * FROM vehicles WHERE id = %d AND owner = %d AND status = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		id, GetPlayerData(playerid, p_id), VEHICLE_STATUS_PLAYER);
	mysql_tquery(mysql, query, "LoadPlayerVehicle", "iffffii", playerid, pos_x, pos_y, pos_z, pos_angle, vw, int);
	return true;
}

stock IsFueledVehicle(vehicleid){
	if(GetVehicleParamsData(GetVehicleParamIndexByModel(GetVehicleModel(vehicleid)), vp_fuel_type) == FUEL_TYPE_NONE) return false;
	return true;
}

stock IsVehicleInRangeOfPoint(vehicleid, Float:radius, Float:x, Float:y, Float:z){
	if(!IsValidVehicle(vehicleid)) return false;

	if(GetVehicleDistanceFromPoint(vehicleid, x, y, z) <= radius) return true;
	return false;
}

//==============================================================================
stock ShowVehicleMenu(playerid){
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !GetPlayerVehicleID(playerid)) return false;
	
	new vehicleid = GetPlayerVehicleID(playerid);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Параметр\t"cl_grey"Статус\n\
		"cl_blue"1. "cl_white"Двигатель\t"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"2. "cl_white"Двери\t"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"3. "cl_white"Фары\t"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"4. "cl_white"Капот\t"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"5. "cl_white"Багажник\t"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"6. "cl_white"Топливо\t"cl_grey"["cl_white"%.02f"cl_grey"]",
		(GetVehicleEngine(vehicleid)) ? (""cl_green"Заведён") : (""cl_dark_red"Заглушен"),
		(!GetVehicleDoors(vehicleid)) ? (""cl_green"Открыты") : (""cl_dark_red"Закрыты"),
		(GetVehicleLights(vehicleid)) ? (""cl_green"Включены") : (""cl_dark_red"Выключены"),
		(GetVehicleBonnet(vehicleid)) ? (""cl_green"Открыт") : (""cl_dark_red"Закрыт"),
		(GetVehicleBoot(vehicleid)) ? (""cl_green"Открыт") : (""cl_dark_red"Закрыт"),
		GetVehicleData(vehicleid, v_fuel));

	Dialog_Open(playerid, Dialog:D_VEHICLE_MENU, DIALOG_STYLE_TABLIST_HEADERS,
		"Управление транспортом", big_string,
		"Выбрать", "Закрыть");
	big_string[0] = EOS;
	return true;
}