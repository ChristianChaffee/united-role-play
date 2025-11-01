
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	CreateDynamicPickup(LICENSING_CENTER_PICKUP_MODEL, 23, LICENSING_CENTER_PICKUP_POS, 
		LICENSING_CENTER_VW, LICENSING_CENTER_INT);

	CreateDynamic3DTextLabel(""cl_blue"ѕолучение лицензий\n"cl_grey"'Left ALT'", -1, 
		LICENSING_CENTER_PICKUP_POS, 15.0, _, _, 1, 
		LICENSING_CENTER_VW, LICENSING_CENTER_INT);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK && IsPlayerInRangeOfPoint(playerid, 1.0, LICENSING_CENTER_PICKUP_POS)){
		LicensingCenter:ShowMenu(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate){
	if(oldstate == PLAYER_STATE_DRIVER && GetPVarType(playerid, "LicensingCenter_Index") && GetPVarType(playerid, "LicensingCenter_VehicleID")){
		SendClientMessage(playerid, COLOR_DARK_RED, "¬ы покинули экзаменационный транспорт и завалили экзамен.");
		LicensingCenter:StopExam(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleDeath(vehicleid, killerid){
	if(GetVehicleData(vehicleid, v_status) == VEHICLE_STATUS_DRIVING_SCHOOL){
		for(new playerid, j = GetPlayerPoolSize(); playerid <= j; playerid++){
			if(GetPVarType(playerid, "LicensingCenter_Index") && GetPVarType(playerid, "LicensingCenter_VehicleID")){
				SendClientMessage(playerid, COLOR_DARK_RED, "¬аш экзаменационный транспорт уничтожен. Ёкзамен не сдан.");
				LicensingCenter:StopExam(playerid);
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleSpawn(vehicleid){
	if(GetVehicleData(vehicleid, v_status) == VEHICLE_STATUS_DRIVING_SCHOOL){
		for(new playerid, j = GetPlayerPoolSize(); playerid <= j; playerid++){
			if(GetPVarType(playerid, "LicensingCenter_Index") && GetPVarType(playerid, "LicensingCenter_VehicleID")){
				SendClientMessage(playerid, COLOR_DARK_RED, "¬аш экзаменационный транспорт уничтожен. Ёкзамен не сдан.");
				LicensingCenter:StopExam(playerid);
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterRCP(playerid){
	if(GetPVarType(playerid, "LicensingCenter_Index") && GetPVarType(playerid, "LicensingCenter_VehicleID") && GetPVarType(playerid, "LicensingCenter_Timer")
		&& GetPlayerState(playerid) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) == GetPVarInt(playerid, "LicensingCenter_VehicleID")){

		if(GetVehicleHealthEx(GetPVarInt(playerid, "LicensingCenter_VehicleID")) < 850.0){
			SendClientMessage(playerid, COLOR_DARK_RED, "Ёкзаменационный транспорт сильно повреждЄн. Ёкзамен не сдан.");
			LicensingCenter:StopExam(playerid);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}

		new 
			license_index = GetPVarInt(playerid, "LicensingCenter_Index"),
			next_cp = GetPVarInt(playerid, "LicensingCenter_CheckpointIndex") + 1;

		DisablePlayerRaceCheckpoint(playerid);

		if(next_cp >= MAX_LICENSING_CENTER_CP_COUNT || 
			(licensing_center_route_cp[license_index][next_cp][0] == 0.0 && licensing_center_route_cp[license_index][next_cp][1] == 0.0 && licensing_center_route_cp[license_index][next_cp][2] == 0.0)){

			GivePlayerLicense(playerid, GetLicensingCenterData(license_index, lc_global_index), GetLicensingCenterData(license_index, lc_time));

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"¬ы сдали практический экзамен и получили \"%s\" на %d дней.",
				GetLicensingCenterData(license_index, lc_category_name),
				GetLicensingCenterData(license_index, lc_time));
			SendClientMessage(playerid, COLOR_GREEN, small_string);
			small_string[0] = EOS;

			LicensingCenter:StopExam(playerid);
		}
		else if(next_cp + 1 >= MAX_LICENSING_CENTER_CP_COUNT || 
			(licensing_center_route_cp[license_index][next_cp + 1][0] == 0.0 && licensing_center_route_cp[license_index][next_cp + 1][1] == 0.0 && licensing_center_route_cp[license_index][next_cp + 1][2] == 0.0)){

			SetPVarInt(playerid, "LicensingCenter_CheckpointIndex", next_cp);
			SetPlayerRaceCheckpoint(playerid, 
				GetLicensingCenterData(license_index, lc_finish_checkpoint), 
				licensing_center_route_cp[license_index][next_cp][0], licensing_center_route_cp[license_index][next_cp][1], licensing_center_route_cp[license_index][next_cp][2],
				0.0, 0.0, 0.0, 
				GetLicensingCenterData(license_index, lc_checkpoint_size));
		}
		else{
			SetPVarInt(playerid, "LicensingCenter_CheckpointIndex", next_cp);
			SetPlayerRaceCheckpoint(playerid, 
				GetLicensingCenterData(license_index, lc_checkpoint), 
				licensing_center_route_cp[license_index][next_cp][0], licensing_center_route_cp[license_index][next_cp][1], licensing_center_route_cp[license_index][next_cp][2],
				licensing_center_route_cp[license_index][next_cp + 1][0], licensing_center_route_cp[license_index][next_cp + 1][1], licensing_center_route_cp[license_index][next_cp + 1][2], 
				GetLicensingCenterData(license_index, lc_checkpoint_size));
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerSecondUpdate(playerid){
	if(GetPVarType(playerid, "LicensingCenter_Index") && GetPVarType(playerid, "LicensingCenter_VehicleID") && GetPVarType(playerid, "LicensingCenter_Timer")){
		SetPVarInt(playerid, "LicensingCenter_Timer", GetPVarInt(playerid, "LicensingCenter_Timer") - 1);
		if(GetPVarInt(playerid, "LicensingCenter_Timer") <= 0){
			SendClientMessage(playerid, COLOR_DARK_RED, "¬рем€ на сдачу экзамена вышло. Ёкзамен не сдан.");
			LicensingCenter:StopExam(playerid);
		}
	}
	return continue(playerid);
}