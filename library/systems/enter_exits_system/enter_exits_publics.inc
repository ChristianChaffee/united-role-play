
forward LoadEnterExits();
public LoadEnterExits(){
	new rows, tick_time = GetTickCount();
	cache_get_row_count(rows);

	if(rows > MAX_ENTER_EXITS_COUNT){
		printf("[LoadEnterExits - ERROR]: Количество записей в базе данных превышает размер массива (%d > %d)", rows, MAX_ENTER_EXITS_COUNT);
		return false;
	}

	for(new i; i < rows; i++){
		cache_get_value_name_int(i, "id", GetEnterExitData(i, ee_id));
		cache_get_value_name(i, "name", GetEnterExitData(i, ee_name));

		cache_get_value_name_float(i, "enter_pos_x", GetEnterExitData(i, ee_enter_pos_x));
		cache_get_value_name_float(i, "enter_pos_y", GetEnterExitData(i, ee_enter_pos_y));
		cache_get_value_name_float(i, "enter_pos_z", GetEnterExitData(i, ee_enter_pos_z));
		cache_get_value_name_float(i, "enter_pos_angle", GetEnterExitData(i, ee_enter_pos_angle));
		cache_get_value_name_int(i, "enter_vw", GetEnterExitData(i, ee_enter_vw));
		cache_get_value_name_int(i, "enter_int", GetEnterExitData(i, ee_enter_int));

		cache_get_value_name_float(i, "exit_pos_x", GetEnterExitData(i, ee_exit_pos_x));
		cache_get_value_name_float(i, "exit_pos_y", GetEnterExitData(i, ee_exit_pos_y));
		cache_get_value_name_float(i, "exit_pos_z", GetEnterExitData(i, ee_exit_pos_z));
		cache_get_value_name_float(i, "exit_pos_angle", GetEnterExitData(i, ee_exit_pos_angle));
		cache_get_value_name_int(i, "exit_vw", GetEnterExitData(i, ee_exit_vw));
		cache_get_value_name_int(i, "exit_int", GetEnterExitData(i, ee_exit_int));

		cache_get_value_name_int(i, "type", GetEnterExitData(i, ee_type));

		//--------------------------------Вход
		small_string[0] = EOS;
		format(small_string, sizeof(small_string), ""cl_white"%s\n"cl_grey"'Left ALT'", GetEnterExitData(i, ee_name));

		CreateDynamic3DTextLabel(small_string, -1,
			GetEnterExitData(i, ee_enter_pos_x), GetEnterExitData(i, ee_enter_pos_y), GetEnterExitData(i, ee_enter_pos_z), 
			ENTER_EXIT_3D_TEXT_DD, _, _, 1, 
			GetEnterExitData(i, ee_enter_vw), GetEnterExitData(i, ee_enter_int));
		small_string[0] = EOS;

		CreateDynamicPickup(ENTER_EXIT_PICKUP_MODEL_ID, 23, 
			GetEnterExitData(i, ee_enter_pos_x), GetEnterExitData(i, ee_enter_pos_y), GetEnterExitData(i, ee_enter_pos_z), 
			GetEnterExitData(i, ee_enter_vw), GetEnterExitData(i, ee_enter_int));
		//-------------------------------- Выход

		CreateDynamic3DTextLabel(""cl_white"Выход\n"cl_grey"'Left ALT'", -1, 
			GetEnterExitData(i, ee_exit_pos_x), GetEnterExitData(i, ee_exit_pos_y), GetEnterExitData(i, ee_exit_pos_z), 
			ENTER_EXIT_3D_TEXT_DD, _, _, 1, 
			GetEnterExitData(i, ee_exit_vw), GetEnterExitData(i, ee_exit_int));

		CreateDynamicPickup(ENTER_EXIT_PICKUP_MODEL_ID, 23, 
			GetEnterExitData(i, ee_exit_pos_x), GetEnterExitData(i, ee_exit_pos_y), GetEnterExitData(i, ee_exit_pos_z), 
			GetEnterExitData(i, ee_exit_vw), GetEnterExitData(i, ee_exit_int));
		//--------------------------------

		TOTAL_ENTER_EXITS++;
	}

	printf("[LoadEnterExits]: Пикапы входа/выхода загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	return true;
}