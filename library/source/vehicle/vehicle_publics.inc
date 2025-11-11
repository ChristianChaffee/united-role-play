
forward VehicleFuelTimer();
public VehicleFuelTimer(){
	for(new i, j = GetVehiclePoolSize(); i <= j; i++){
		if(!IsValidVehicle(i)) continue;
		if(!GetVehicleEngine(i)) continue;

		switch(GetVehicleSpeed(i)){
			case 0..20: SetVehicleFuel(i, GetVehicleData(i, v_fuel) - 1.0);
			case 21..100: SetVehicleFuel(i, GetVehicleData(i, v_fuel) - 1.5);
			default: SetVehicleFuel(i, GetVehicleData(i, v_fuel) - 2.0);
		}

		if(GetVehicleData(i, v_fuel) <= 0.0){
			SetVehicleEngine(i, false);
			SetVehicleFuel(i, 0.0);
		}
	}
}

forward ShowPlayerCars(playerid);
public ShowPlayerCars(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return SendErrorMessage(playerid, "У Вас нет личного транспорта.");

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Модель\t"cl_grey"Статус");

	for(new i, temp_id, temp_model_id; i < rows; i++){
		cache_get_value_name_int(i, "id", temp_id);
		cache_get_value_name_int(i, "model_id", temp_model_id);

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_lime"%d. "cl_white"%s "cl_grey"[ID: %d]\t%s",
			big_string, i + 1,
			GetVehicleParamsData(GetVehicleParamIndexByModel(temp_model_id), vp_name), temp_id,
			(FindVehicleById(temp_id) == INVALID_VEHICLE_ID) ? (""cl_light_red"Не загружена") : (""cl_green"Загружена"));
		
		player_listitems[playerid][i] = temp_id;
	}
	Dialog_Open(playerid, Dialog:D_PLAYER_CARS_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Мой транспорта", big_string,
		"Выбрать", "Закрыть");
	big_string[0] = EOS;
	return true;
}

forward LoadPlayerVehicle(playerid);
public LoadPlayerVehicle(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return SendErrorMessage(playerid, "При загрузке транспорта произошла ошибка.");

	new Float:temp_pos[4];
	cache_get_value_name_float(0, "pos_x", temp_pos[0]);
	cache_get_value_name_float(0, "pos_y", temp_pos[1]);
	cache_get_value_name_float(0, "pos_z", temp_pos[2]);
	cache_get_value_name_float(0, "pos_angle", temp_pos[3]);

	if(temp_pos[0] == 0.0 && temp_pos[1] == 0.0 && temp_pos[2] == 0.0 && temp_pos[3] == 0.0){
		SendClientMessage(playerid, COLOR_LIGHT_RED, "Данный транспорт находится на штраф-стоянке.");
		return false;
	}

	return true;
}