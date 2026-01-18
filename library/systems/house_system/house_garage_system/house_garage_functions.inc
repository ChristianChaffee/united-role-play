
stock House:GarageUpdate(garage_index){
	if(garage_index < 0 || garage_index >= MAX_GARAGES) return false;

	if(IsValidDynamicPickup(GetGarageData(garage_index, g_enter_pickup))){
		DestroyDynamicPickup(GetGarageData(garage_index, g_enter_pickup));
	}
	if(IsValidDynamic3DTextLabel(GetGarageData(garage_index, g_enter_text))){
		DestroyDynamic3DTextLabel(GetGarageData(garage_index, g_enter_text));
	}
	if(IsValidDynamic3DTextLabel(GetGarageData(garage_index, g_exit_text))){
		DestroyDynamic3DTextLabel(GetGarageData(garage_index, g_exit_text));
	}
	if(IsValidDynamicPickup(GetGarageData(garage_index, g_repair_pickup))){
		DestroyDynamicPickup(GetGarageData(garage_index, g_repair_pickup));
	}
	if(IsValidDynamic3DTextLabel(GetGarageData(garage_index, g_repair_text))){
		DestroyDynamic3DTextLabel(GetGarageData(garage_index, g_repair_text));
	}

	GetGarageData(garage_index, g_enter_pickup) = CreateDynamicPickup(GARAGE_ENTER_PICKUP_MODEL, 23, 
		GetGarageData(garage_index, g_pos_x), 
		GetGarageData(garage_index, g_pos_y), 
		GetGarageData(garage_index, g_pos_z), 
		0, 0);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_lime"[Гараж дома №%d]\n\
		"cl_grey"Пешком: "cl_white"'Left ALT'\n\
		"cl_grey"В транспорте: "cl_white"'H'",
		GetGarageData(garage_index, g_id));

	GetGarageData(garage_index, g_enter_text) = CreateDynamic3DTextLabel(small_string, -1, 
		GetGarageData(garage_index, g_pos_x), 
		GetGarageData(garage_index, g_pos_y), 
		GetGarageData(garage_index, g_pos_z), 
		15.0, _, _, 1, 
		0, 0);
	small_string[0] = EOS;

	GetGarageData(garage_index, g_exit_text) = CreateDynamic3DTextLabel(""cl_grey"[Выход]\n\
		"cl_grey"Пешком: "cl_white"'Left ALT'\n\
		"cl_grey"В транспорте: "cl_white"'H'",
		-1,
		GARAGE_TP_POS,
		15.0, _, _, 1,
		GetGarageData(garage_index, g_id), GARAGE_TP_INT);

	GetGarageData(garage_index, g_repair_pickup) = CreateDynamicPickup(GARAGE_REPAIR_PICKUP_MODEL, 23, 
		GARAGE_REPAIR_POS, 
		GetGarageData(garage_index, g_id), GARAGE_TP_INT);

	GetGarageData(garage_index, g_repair_text) = CreateDynamic3DTextLabel(""cl_lime"[Ремонт транспорта]\n\
		"cl_grey"'Left ALT'",
		-1,
		GARAGE_REPAIR_POS,
		15.0, _, _, 1,
		GetGarageData(garage_index, g_id), GARAGE_TP_INT);
	return true;
}

stock House:IsPlayerHaveAccessToGarage(playerid, garage_index){
	if(garage_index < 0 || garage_index >= MAX_GARAGES) return false;

	if(GetPlayerData(playerid, p_tenant_house_id) == GetGarageData(garage_index, g_house_id)) return true;
	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_id) != GetGarageData(garage_index, g_house_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;

		return true;
	}
	return false;
}

stock House:GarageEnter(playerid, garage_index){
	if(garage_index < 0 || garage_index >= MAX_GARAGES) return false;
	if(!GetGarageData(garage_index, g_id)) return false;

	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER){
		new vehicleid = GetPlayerVehicleID(playerid);

		SetVehiclePos(vehicleid, GARAGE_TP_POS);
		SetVehicleZAngle(vehicleid, GARAGE_TP_ANGLE);
		SetVehicleVirtualWorld(vehicleid, GetGarageData(garage_index, g_id));
		LinkVehicleToInterior(vehicleid, GARAGE_TP_INT);

		SetCameraBehindPlayer(playerid);

		for(new i, j = GetPlayerPoolSize(); i <= j; i++){
			if(!player_logged[i]) continue;
			if(GetPlayerVehicleID(i) != vehicleid) continue;

			SetPlayerVirtualWorld(i, GetGarageData(garage_index, g_id));
			SetPlayerInterior(i, GARAGE_TP_INT);
		}
	}
	else{
		SetPlayerPos(playerid, GARAGE_TP_POS);
		SetPlayerFacingAngle(playerid, GARAGE_TP_ANGLE);
		SetPlayerVirtualWorld(playerid, GetGarageData(garage_index, g_id));
		SetPlayerInterior(playerid, GARAGE_TP_INT);

		SetCameraBehindPlayer(playerid);
	}
	return true;
}

stock House:GarageExit(playerid, garage_index){
	if(garage_index < 0 || garage_index >= MAX_GARAGES) return false;
	if(!GetGarageData(garage_index, g_id)) return false;

	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER){
		new vehicleid = GetPlayerVehicleID(playerid);

		SetVehiclePos(vehicleid, GetGarageData(garage_index, g_pos_x), GetGarageData(garage_index, g_pos_y), GetGarageData(garage_index, g_pos_z));
		SetVehicleVirtualWorld(vehicleid, 0);
		LinkVehicleToInterior(vehicleid, 0);

		SetCameraBehindPlayer(playerid);

		for(new i, j = GetPlayerPoolSize(); i <= j; i++){
			if(!player_logged[i]) continue;
			if(GetPlayerVehicleID(i) != vehicleid) continue;

			SetPlayerVirtualWorld(i, 0);
			SetPlayerInterior(i, 0);
		}
	}
	else{
		SetPlayerPos(playerid, GetGarageData(garage_index, g_pos_x), GetGarageData(garage_index, g_pos_y), GetGarageData(garage_index, g_pos_z));
		SetPlayerVirtualWorld(playerid, 0);
		SetPlayerInterior(playerid, 0);

		SetCameraBehindPlayer(playerid);
	}
	return true;
}

stock House:ShowGarageRepair(playerid, garage_index){
	if(garage_index < 0 || garage_index >= MAX_GARAGES) return false;

	cef_emit_event(playerid, "showGarageRepair");
	cef_emit_event(playerid, "clearGarageRepairVehicles");

	for(new i, j = GetVehiclePoolSize(), Float:veh_health; i <= j; i++){
		if(!IsValidVehicle(i)) continue;
		if(GetVehicleVirtualWorld(i) != GetGarageData(garage_index, g_id)) continue;
		if(!IsVehicleInRangeOfPoint(i, 50.0, GARAGE_REPAIR_POS)) continue;

		GetVehicleHealth(i, veh_health);

		cef_emit_event(playerid, "addGarageRepairVehicle",
			CEFSTR(GetVehicleParamsData(GetVehicleParamIndexByModel(GetVehicleModel(i)), vp_name)),
			CEFINT(floatround((veh_health * 100.0) / 1000.0)),
			CEFINT(floatround((100.0 - (veh_health * 100.0) / 1000.0) * GARAGE_REPAIR_PERCENT_PRICE)),
			CEFINT(i));
	}

	return true;
}

stock House:HideGarageRepair(playerid){
	cef_emit_event(playerid, "hideGarageRepair");
	return true;
}