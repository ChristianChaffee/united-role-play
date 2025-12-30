
forward LoadVehicles();
public LoadVehicles(){
	new rows,
		tick_time = GetTickCount();

	cache_get_row_count(rows);

	if(rows >= MAX_VEHICLES){
		printf("[LoadVehicles - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_VEHICLES);
		return false;
	}

	for(new i, temp_vehicle_data[vehicle_info_struct], vehicleid; i < rows; i++){
		cache_get_value_name_int(i, "id", temp_vehicle_data[v_id]);
		cache_get_value_name_int(i, "model_id", temp_vehicle_data[v_model_id]);
		cache_get_value_name_int(i, "status", temp_vehicle_data[v_status]);
		cache_get_value_name_int(i, "owner", temp_vehicle_data[v_owner]);
		cache_get_value_name_int(i, "price", temp_vehicle_data[v_price]);
		cache_get_value_name_int(i, "color_1", temp_vehicle_data[v_color_1]);
		cache_get_value_name_int(i, "color_2", temp_vehicle_data[v_color_2]);

		cache_get_value_name_float(i, "pos_x", temp_vehicle_data[v_pos_spawn_x]);
		cache_get_value_name_float(i, "pos_y", temp_vehicle_data[v_pos_spawn_y]);
		cache_get_value_name_float(i, "pos_z", temp_vehicle_data[v_pos_spawn_z]);
		cache_get_value_name_float(i, "pos_angle", temp_vehicle_data[v_pos_spawn_angle]);
		cache_get_value_name_int(i, "pos_vw", temp_vehicle_data[v_vw]);
		cache_get_value_name_int(i, "pos_int", temp_vehicle_data[v_int]);

		cache_get_value_name_float(i, "fuel", temp_vehicle_data[v_fuel]);

		vehicleid = CreateVehicle(temp_vehicle_data[v_model_id], 
			temp_vehicle_data[v_pos_spawn_x], 
			temp_vehicle_data[v_pos_spawn_y], 
			temp_vehicle_data[v_pos_spawn_z], 
			temp_vehicle_data[v_pos_spawn_angle], 
			temp_vehicle_data[v_color_1], temp_vehicle_data[v_color_2], 1000);

		SetVehicleVirtualWorld(vehicleid, temp_vehicle_data[v_vw]);
		LinkVehicleToInterior(vehicleid, temp_vehicle_data[v_int]);

		vehicle_info[vehicleid] = temp_vehicle_data;
	}

	printf("[LoadVehicles]: Транспорт загружен <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	load_info[loaded_vehicles] = true;
	return true;
}

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

forward CheckPlayerVehicle(playerid);
public CheckPlayerVehicle(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return SendErrorMessage(playerid, "При загрузке транспорта произошла ошибка.");

	new Float:temp_pos[4], temp_id;
	cache_get_value_name_int(0, "id", temp_id);
	cache_get_value_name_float(0, "pos_x", temp_pos[0]);
	cache_get_value_name_float(0, "pos_y", temp_pos[1]);
	cache_get_value_name_float(0, "pos_z", temp_pos[2]);
	cache_get_value_name_float(0, "pos_angle", temp_pos[3]);

	if(temp_pos[0] == 0.0 && temp_pos[1] == 0.0 && temp_pos[2] == 0.0 && temp_pos[3] == 0.0){
		SendClientMessage(playerid, COLOR_LIGHT_RED, "Данный транспорт находится на штраф-стоянке.");
		return false;
	}
	
	CreatePlayerVehicle(playerid, temp_id);
	return true;
}

forward LoadPlayerVehicle(playerid, Float:pos_x, Float:pos_y, Float:pos_z, Float:pos_angle, vw, int);
public LoadPlayerVehicle(playerid, Float:pos_x, Float:pos_y, Float:pos_z, Float:pos_angle, vw, int){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return SendErrorMessage(playerid, "При загрузке транспорта произошла ошибка.");

	new temp_vehicle_data[vehicle_info_struct];

	cache_get_value_name_int(0, "id", temp_vehicle_data[v_id]);
	cache_get_value_name_int(0, "model_id", temp_vehicle_data[v_model_id]);
	cache_get_value_name_int(0, "status", temp_vehicle_data[v_status]);
	cache_get_value_name_int(0, "owner", temp_vehicle_data[v_owner]);
	cache_get_value_name_int(0, "price", temp_vehicle_data[v_price]);
	cache_get_value_name_int(0, "color_1", temp_vehicle_data[v_color_1]);
	cache_get_value_name_int(0, "color_2", temp_vehicle_data[v_color_2]);

	cache_get_value_name_float(0, "pos_x", temp_vehicle_data[v_pos_spawn_x]);
	cache_get_value_name_float(0, "pos_y", temp_vehicle_data[v_pos_spawn_y]);
	cache_get_value_name_float(0, "pos_z", temp_vehicle_data[v_pos_spawn_z]);
	cache_get_value_name_float(0, "pos_angle", temp_vehicle_data[v_pos_spawn_angle]);
	cache_get_value_name_int(0, "pos_vw", temp_vehicle_data[v_vw]);
	cache_get_value_name_int(0, "pos_int", temp_vehicle_data[v_int]);

	cache_get_value_name_float(0, "fuel", temp_vehicle_data[v_fuel]);

	if(pos_x != 0.0 && pos_y != 0.0 && pos_z != 0.0 && pos_angle != 0.0){
		temp_vehicle_data[v_pos_spawn_x] = pos_x;
		temp_vehicle_data[v_pos_spawn_y] = pos_y;
		temp_vehicle_data[v_pos_spawn_z] = pos_z;
		temp_vehicle_data[v_pos_spawn_angle] = pos_angle;
		temp_vehicle_data[v_vw] = vw;
		temp_vehicle_data[v_int] = int;
	}

	new vehicleid = CreateVehicle(temp_vehicle_data[v_model_id], 
		temp_vehicle_data[v_pos_spawn_x], 
		temp_vehicle_data[v_pos_spawn_y], 
		temp_vehicle_data[v_pos_spawn_z], 
		temp_vehicle_data[v_pos_spawn_angle], 
		temp_vehicle_data[v_color_1], 
		temp_vehicle_data[v_color_2], -1, 0);

	vehicle_info[vehicleid] = temp_vehicle_data;

	SetVehicleStatus(vehicleid, VEHICLE_STATUS_PLAYER);
	return true;
}