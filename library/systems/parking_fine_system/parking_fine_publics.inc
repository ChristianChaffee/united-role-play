
forward CheckPlayerParkFineVehicles(playerid);
public CheckPlayerParkFineVehicles(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return SendErrorMessage(playerid, "У Вас нет транспорта на штраф-стоянке.");

	new count;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Модель\t"cl_grey"Стоимость возврата");

	for(new i, temp_id, temp_model_id; i < rows; i++){
		cache_get_value_name_int(i, "id", temp_id);
		cache_get_value_name_int(i, "model_id", temp_model_id);

		if(FindVehicleById(temp_id) != INVALID_VEHICLE_ID) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_lime"%d. "cl_white"%s "cl_grey"[ID: %d]\t"cl_green"%d$",
			big_string, count + 1,
			GetVehicleParamsData(GetVehicleParamIndexByModel(temp_model_id), vp_name), temp_id, PARKING_FINE_PRICE);

		player_listitems[playerid][count] = temp_id;
		count++;
	}

	if(!count){
		big_string[0] = EOS;
		SendErrorMessage(playerid, "У Вас нет транспорта на штраф-стоянке.");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_PARK_FINE_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Штраф-стоянка", big_string,
		"Забрать", "Закрыть");
	big_string[0] = EOS;
	return true;
}