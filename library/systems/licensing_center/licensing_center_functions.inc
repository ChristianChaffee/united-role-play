
stock LicensingCenter_ShowMenu(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string), ""cl_grey"Лицензия\t"cl_grey"Стоимость");
	for(new i; i < LICENSINGS_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_grey"["cl_green"%d$"cl_grey"]",
			big_string, i + 1,
			GetLicensingCenterData(i, lc_category_name), GetLicensingCenterData(i, lc_category_price));
	}

	Dialog_Open(playerid, Dialog:D_LICENSING_CENTER_MENU, DIALOG_STYLE_TABLIST_HEADERS,
		"Центр лицензирования", big_string,
		"Выбрать", "Закрыть");
	big_string[0] = EOS;
	return true;
}

stock LicensingCenter:ShowAccept(playerid){
	if(!GetPVarType(playerid, "LicensingCenter_Index")) return false;

	new 
		license_index = GetPVarInt(playerid, "LicensingCenter_Index"),
		license_global_index = GetLicensingCenterData(license_index, lc_global_index);

	big_string[0] = EOS;
	if(player_info[playerid][p_licenses][license_global_index] && player_info[playerid][p_licenses_times][license_global_index] > 0){
		format(big_string, sizeof(big_string),
			""cl_blue"- "cl_white"У Вас уже есть "cl_grey"\"%s\""cl_white", вы действительно хотите продлить её на "cl_grey"%d дней "cl_white"за "cl_green"%d$"cl_white"?\n\n\n\
			\\c"cl_grey"[Сдача практического экзамена не требуется]",
			GetLicensingCenterData(license_index, lc_category_name),
			GetLicensingCenterData(license_index, lc_time),
			GetLicensingCenterData(license_index, lc_category_price));
	}
	else{
		format(big_string, sizeof(big_string),
			""cl_blue"- "cl_white"Вы действительно хотите начать экзамен на "cl_grey"\"%s\" (%d дней) "cl_white"за "cl_green"%d$"cl_white"?\n\n\n\
			\\c"cl_grey"[Требуется сдача практического экзамена. В случе не сдачи денежные средства возвращены не будут]",
			GetLicensingCenterData(license_index, lc_category_name),
			GetLicensingCenterData(license_index, lc_time),
			GetLicensingCenterData(license_index, lc_category_price));
	}

	Dialog_Open(playerid, Dialog:D_LICENSING_CENTER_ACCEPT, DIALOG_STYLE_MSGBOX,
		"Центр лицензирования", big_string,
		"Да", "Нет");
	big_string[0] = EOS;
	return true;
}

//==============================================================================
stock LicensingCenter:StartExam(playerid){
	if(!GetPVarType(playerid, "LicensingCenter_Index")) return false;

	new 
		license_index = GetPVarInt(playerid, "LicensingCenter_Index"),
		vehicle_model_id = licensing_center_info[license_index][lc_random_vehicle_model_id][random(LICENSINGS_RANDOM_VEH_COUNT)];

	while(!vehicle_model_id){
		vehicle_model_id = licensing_center_info[license_index][lc_random_vehicle_model_id][random(LICENSINGS_RANDOM_VEH_COUNT)];
	}

	new Float:pos[4];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);

	SetPVarFloat(playerid, "LicensingCenter_PosX", pos[0]);
	SetPVarFloat(playerid, "LicensingCenter_PosY", pos[1]);
	SetPVarFloat(playerid, "LicensingCenter_PosZ", pos[2]);
	SetPVarFloat(playerid, "LicensingCenter_PosAngle", pos[3]);
	SetPVarInt(playerid, "LicensingCenter_VW", GetPlayerVirtualWorld(playerid));
	SetPVarInt(playerid, "LicensingCenter_Int", GetPlayerInterior(playerid));

	SetPlayerPos(playerid,
		GetLicensingCenterData(license_index, lc_veh_spawn_x),
		GetLicensingCenterData(license_index, lc_veh_spawn_y),
		GetLicensingCenterData(license_index, lc_veh_spawn_z));
	SetPlayerVirtualWorld(playerid, playerid + 1);
	SetPlayerInterior(playerid, 0);

	new vehicleid = CreateVehicle(vehicle_model_id, 
		GetLicensingCenterData(license_index, lc_veh_spawn_x), 
		GetLicensingCenterData(license_index, lc_veh_spawn_y), 
		GetLicensingCenterData(license_index, lc_veh_spawn_z), 
		GetLicensingCenterData(license_index, lc_veh_spawn_angle), 
		3, 3, 1000);

	SetVehicleVirtualWorld(vehicleid, playerid + 1);
	LinkVehicleToInterior(vehicleid, 0);
	SetVehicleStatus(vehicleid, VEHICLE_STATUS_DRIVING_SCHOOL);

	PutPlayerInVehicle(playerid, vehicleid, 0);
	SetCameraBehindPlayer(playerid);

	SetPVarInt(playerid, "LicensingCenter_VehicleID", vehicleid);
	SetPVarInt(playerid, "LicensingCenter_Timer", LICENSINGS_EXAM_MAX_TIME * 60);
	SetPVarInt(playerid, "LicensingCenter_CheckpointIndex", 0);

	Dialog_Open(playerid, Dialog:D_NULL, DIALOG_STYLE_MSGBOX,
		"Практический экзамен", 
		""cl_blue"- "cl_white"Заведите транспорт используя клавишу "cl_grey"\"2\", затем начните движения по чекпоинтам.\n\
		На дачу экзамена даётся "#LICENSINGS_EXAM_MAX_TIME" минут.\n\n\
		\\c"cl_grey"[Экзамен будет провелаен в следующих случаях]\n\
		"cl_grey"1. "cl_white"Сдача экзамена более "#LICENSINGS_EXAM_MAX_TIME" минут.\n\
		"cl_grey"2. "cl_white"Выход из экзаменационного транспорта.\n\
		"cl_grey"3. "cl_white"Исправность рабочего транспорта менее 85 процентов.",
		"Понятно", "");
	big_string[0] = EOS;

	SetPlayerRaceCheckpoint(playerid, 
		GetLicensingCenterData(license_index, lc_checkpoint), 
		licensing_center_route_cp[license_index][0][0], licensing_center_route_cp[license_index][0][1], licensing_center_route_cp[license_index][0][2],
		licensing_center_route_cp[license_index][1][0], licensing_center_route_cp[license_index][1][1], licensing_center_route_cp[license_index][1][2], 
		GetLicensingCenterData(license_index, lc_checkpoint_size));
	return true;
}

stock LicensingCenter_StopExam(playerid, teleport = true){

	RemovePlayerFromVehicle(playerid);

	if(teleport){
		SetPlayerPos(playerid,
			GetPVarFloat(playerid, "LicensingCenter_PosX"),
			GetPVarFloat(playerid, "LicensingCenter_PosY"),
			GetPVarFloat(playerid, "LicensingCenter_PosZ"));
		SetPlayerFacingAngle(playerid,
			GetPVarFloat(playerid, "LicensingCenter_PosAngle"));
		SetPlayerVirtualWorld(playerid, GetPVarInt(playerid, "LicensingCenter_VW"));
		SetPlayerInterior(playerid, GetPVarInt(playerid, "LicensingCenter_Int"));
		SetCameraBehindPlayer(playerid);
	}

	if(IsValidVehicle(GetPVarInt(playerid, "LicensingCenter_VehicleID"))){
		DestroyVehicle(GetPVarInt(playerid, "LicensingCenter_VehicleID"));
	}

	DisablePlayerCheckpoint(playerid);

	DeletePVar(playerid, "LicensingCenter_Index");
	DeletePVar(playerid, "LicensingCenter_PosX");
	DeletePVar(playerid, "LicensingCenter_PosY");
	DeletePVar(playerid, "LicensingCenter_PosZ");
	DeletePVar(playerid, "LicensingCenter_PosAngle");
	DeletePVar(playerid, "LicensingCenter_VW");
	DeletePVar(playerid, "LicensingCenter_Int");
	DeletePVar(playerid, "LicensingCenter_VehicleID");
	DeletePVar(playerid, "LicensingCenter_Timer");
	DeletePVar(playerid, "LicensingCenter_CheckpointIndex");
	return true;
}